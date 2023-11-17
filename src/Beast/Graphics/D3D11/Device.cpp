#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Device.h"
#include "Beast/Graphics/D3D11/Context.h"

#include "Beast/Common/UUIDGenerator.h"

#include <d3dcompiler.h>

#pragma comment(lib, "DXGI.lib")

namespace be::graphics::d3d11
{
    static wrl::ComPtr<ID3DBlob> LoadShaderByteCode(const FilesystemPath& shaderFilepath)
    {
        if (!std::filesystem::exists(shaderFilepath))
        {
            throw std::runtime_error("placeholder");
        }

        wrl::ComPtr<ID3DBlob> shaderByteCode;
        CheckResult(D3DReadFileToBlob(shaderFilepath.wstring().c_str(), &shaderByteCode));

        return shaderByteCode;
    }

    std::vector<IDXGIAdapter*> EnumerateAdapters(void)
    {
        IDXGIAdapter* pAdapter;
        std::vector<IDXGIAdapter*> vAdapters;
        IDXGIFactory1* pFactory = NULL;

        // Create a DXGIFactory object.
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)))
        {
            return vAdapters;
        }

        for (UINT i = 0;
             pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND;
             ++i)
        {
            vAdapters.push_back(pAdapter);
        }

        if (pFactory)
        {
            pFactory->Release();
        }

        return vAdapters;
    }

    Device::Device(const IWindow& window)
    {
        const auto hwnd = window.GetHandle();

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 0u;
        sd.BufferDesc.Height = 0u;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0u;
        sd.BufferDesc.RefreshRate.Denominator = 0u;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1u;
        sd.SampleDesc.Quality = 0u;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1u;
        sd.OutputWindow = hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0u;

        const auto result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0u,
            D3D11_SDK_VERSION,
            &sd,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_context
        );
        CheckResult(result);

        m_renderContext = MakeUnique<Context>(*this, m_swapChain.Get(), m_context.Get(), CreateRenderTargetView(*m_swapChain.Get()));

        const auto adapters = EnumerateAdapters();
        for (auto* pAdapter : adapters)
        {
            IDXGIOutput* pOutput = NULL;

            CheckResult(pAdapter->EnumOutputs(0, &pOutput));
            
            UINT numModes = 0;
            DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

            // Get the number of elements
            CheckResult(pOutput->GetDisplayModeList(format, 0, &numModes, NULL));

            std::vector<DXGI_MODE_DESC> displayModes(numModes);

            // Get the list
            CheckResult(pOutput->GetDisplayModeList(format, 0, &numModes, displayModes.data()));

            for (const auto& mode : displayModes)
            {
                BE_DEBUG_LOG_INFO("[width: {}, height: {}]", mode.Width, mode.Height);
            }
        }
    }

    Device::~Device()
    {
        // Make sure this is destroyed first as it internally references "*this"
        m_renderContext.reset();
    }

    graphics::VertexBuffer Device::CreateVertexBuffer(uint32 stride, uint32 maxSize)
    {
        D3D11_BUFFER_DESC bufferDescriptor = {
            .ByteWidth = maxSize,
            .Usage = D3D11_USAGE_DYNAMIC,
            .BindFlags = D3D11_BIND_VERTEX_BUFFER,
            .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
            .MiscFlags = 0,
            .StructureByteStride = stride,
        };
        wrl::ComPtr<ID3D11Buffer> bufferPtr;
        CheckResult(m_device->CreateBuffer(&bufferDescriptor, nullptr, &bufferPtr));

        d3d11::VertexBuffer buffer{std::move(bufferPtr), stride};
        graphics::VertexBuffer bufferRef{.id = GenerateUUID4()};
        
        m_vertexBuffers.insert({bufferRef.id, std::move(buffer)});

        return bufferRef;
    }

    graphics::VertexShader Device::CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout)
    {
        wrl::ComPtr<ID3D11VertexShader> shaderPtr;
        wrl::ComPtr<ID3D11InputLayout> layoutPtr;
        wrl::ComPtr<ID3DBlob> shaderByteCode = LoadShaderByteCode(filepath);

        // Create Shader
        {
            CheckResult(m_device->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));
        }

        // Create Input Layout
        {
            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
            inputElements.reserve(inputLayout.vertexAttributes.size());
            
            for (const auto& attribute : inputLayout.vertexAttributes)
            {
                inputElements.emplace_back(D3D11_INPUT_ELEMENT_DESC{
                    .SemanticName = attribute.name,
                    .SemanticIndex = attribute.index,
                    .Format = ConvertInputElementFormat(attribute.format),
                    .InputSlot = 0u,
                    .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
                    .InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
                    .InstanceDataStepRate = 0u,
                });
            }
            
            CheckResult(m_device->CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &layoutPtr));
        }

        d3d11::VertexShader shader{std::move(shaderPtr), std::move(layoutPtr)};
        graphics::VertexShader shaderRef{.id = GenerateUUID4()};

        m_vertexShaders.insert({shaderRef.id, std::move(shader)});

        return shaderRef;
    }

    graphics::PixelShader Device::CreatePixelShader(const FilesystemPath& filepath)
    {
        wrl::ComPtr<ID3D11PixelShader> shaderPtr;
        wrl::ComPtr<ID3DBlob> shaderByteCode = LoadShaderByteCode(filepath);        
        CheckResult(m_device->CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));

        d3d11::PixelShader shader{std::move(shaderPtr)};
        graphics::PixelShader shaderRef{.id = GenerateUUID4()};

        m_pixelShaders.insert({shaderRef.id, std::move(shader)});

        return shaderRef;
    }

    const IRenderContext& Device::GetContext() const noexcept
    {
        return *m_renderContext;
    }

    wrl::ComPtr<ID3D11RenderTargetView> Device::CreateRenderTargetView(IDXGISwapChain& swapChain) const
    {
        wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;

        wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
        CheckResult(swapChain.GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
        CheckResult(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

        return renderTargetView;
    }

    const d3d11::VertexBuffer& Device::GetBuffer(graphics::VertexBuffer bufferRef) const
    {
        return m_vertexBuffers.at(bufferRef.id);
    }

    const d3d11::VertexShader& Device::GetShader(graphics::VertexShader shaderRef) const
    {
        return m_vertexShaders.at(shaderRef.id);
    }

    const d3d11::PixelShader& Device::GetShader(graphics::PixelShader shaderRef) const
    {
        return m_pixelShaders.at(shaderRef.id);
    }

    void Device::Run()
    {
        //static const auto vertexBuffer = CreateVertexBuffer();
        //static const std::vector<Vertex> vertices = {
        //    {
        //        .position = {-0.5f, -0.5f},
        //        .color = {1.0f, 0.0f, 0.0f, 1.0f},
        //    },
        //    {
        //        .position = {0.0f, 0.5f},
        //        .color = {0.0f, 1.0f, 0.0f, 1.0f},
        //    },
        //    {
        //        .position = {0.5f, -0.5f},
        //        .color = {0.0f, 0.0f, 1.0f, 1.0f},
        //    },
        //};
        //auto& buffer = m_vertexBuffers.at(vertexBuffer.id);
        //buffer.Update(*m_context.Get(), vertices);
        //buffer.Bind(*m_context.Get());

        ///*const float color[] = {0.0f, 0.0f, 0.0f, 1.0f};

        //static const auto renderTargetView = CreateRenderTargetView(*m_swapChain.Get());
        //m_context->ClearRenderTargetView(renderTargetView.Get(), color);*/

        ////static const std::vector<Vertex> vertices = {
        ////    {
        ////        .position = {0.5f, -0.5f},
        ////        //.color = {1.0f, 0.0f, 0.0f, 1.0f},
        ////    },
        ////    {
        ////        .position = {-0.5f, -0.5f},
        ////        //.color = {0.0f, 1.0f, 0.0f, 1.0f},
        ////    },
        ////    {
        ////        .position = {0.0f, 0.5f},
        ////        //.color = {0.0f, 0.0f, 1.0f, 1.0f},
        ////    },
        ////};

        ////wrl::ComPtr<ID3D11Buffer> vertexBuffer;

        ////D3D11_BUFFER_DESC bufferDescriptor = {};
        ////bufferDescriptor.ByteWidth = sizeof(Vertex) * 3; // This must be the MAX size of the buffer
        ////bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
        ////bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        ////bufferDescriptor.CPUAccessFlags = 0;
        ////bufferDescriptor.MiscFlags = 0;
        ////bufferDescriptor.StructureByteStride = sizeof(Vertex);

        ////D3D11_SUBRESOURCE_DATA bufferData = {};
        ////bufferData.pSysMem = vertices.data();

        ////CheckResult(m_device->CreateBuffer(&bufferDescriptor, &bufferData, &vertexBuffer));
        ////const UINT stride = sizeof(Vertex);
        ////const UINT offset = 0u;
        ////m_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

        ////// VERTEX SHADER
        ////wrl::ComPtr<ID3D11VertexShader> vShader;
        ////wrl::ComPtr<ID3DBlob> shaderByteCode;
        ////CheckResult(D3DReadFileToBlob(L"VertexShader.cso", &shaderByteCode));

        ////CheckResult(m_device->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &vShader));
        ////m_context->VSSetShader(vShader.Get(), nullptr, 0);

        ////D3D11_INPUT_ELEMENT_DESC decs{
        ////    .SemanticName = "POSITION",
        ////    .SemanticIndex = 0,
        ////    .Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
        ////    .InputSlot = 0,
        ////    .InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
        ////    .InstanceDataStepRate = 0,
        ////};

        ////wrl::ComPtr<ID3D11InputLayout> inputLayout;
        ////CheckResult(m_device->CreateInputLayout(&decs, 1u, shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &inputLayout));
        ////m_context->IASetInputLayout(inputLayout.Get());

        ////// PIXEL SHADER
        ////wrl::ComPtr<ID3D11PixelShader> pShader;
        ////CheckResult(D3DReadFileToBlob(L"PixelShader.cso", &shaderByteCode));

        ////CheckResult(m_device->CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &pShader));
        ////m_context->PSSetShader(pShader.Get(), nullptr, 0u);

        ////// RENDER TARGET
        //////m_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        ///*D3D11_VIEWPORT d3dViewport{};
        //d3dViewport.Width = 800.0f;
        //d3dViewport.Height = 600.0f;
        //d3dViewport.MinDepth = 0.0f;
        //d3dViewport.MaxDepth = 1.0f;
        //d3dViewport.TopLeftX = 0.0f;
        //d3dViewport.TopLeftY = 0.0f;
        //m_context->RSSetViewports(1, &d3dViewport);

        //m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        //m_context->Draw(3u, 0u);*/

        //static const InputLayout layout{
        //    .vertexAttributes = {
        //        {"POSITION", InputLayout::VertexAttribute::Format::Vec2},
        //        {"COLOR", InputLayout::VertexAttribute::Format::Vec4},
        //    },
        //};
        //static const auto vertexShaderRef = CreateVertexShader("VertexShader.cso", layout);
        //auto& vertexShader = m_vertexShaders.at(vertexShaderRef.id);
        //vertexShader.Bind(*m_context.Get());

        //static const auto pixelShaderRef = CreatePixelShader("PixelShader.cso");
        //auto& pixelShader = m_pixelShaders.at(pixelShaderRef.id);
        //pixelShader.Bind(*m_context.Get());

        //D3D11_VIEWPORT d3dViewport{};
        //d3dViewport.Width = 800.0f;
        //d3dViewport.Height = 600.0f;
        //d3dViewport.MinDepth = 0.0f;
        //d3dViewport.MaxDepth = 1.0f;
        //d3dViewport.TopLeftX = 0.0f;
        //d3dViewport.TopLeftY = 0.0f;
        //m_context->RSSetViewports(1, &d3dViewport);

        ///*static const auto renderTargetView = CreateRenderTargetView(*m_swapChain.Get());

        //m_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), m_view.Get());
        //Color color{0.0f, 0.0f, 0.0f, 1.0f};
        //m_context->ClearRenderTargetView(renderTargetView.Get(), color.Data());*/

        //m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        //m_context->Draw(3, 0);
    }
} // namespace be::graphics::d3d11
