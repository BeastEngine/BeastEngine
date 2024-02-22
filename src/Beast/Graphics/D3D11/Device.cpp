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
        wrl::ComPtr<ID3DBlob> shaderByteCode;
        BE_DX_CALL(D3DReadFileToBlob(shaderFilepath.wstring().c_str(), &shaderByteCode));

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
        BE_DX_CALL(result);

        m_renderContext = MakeUnique<Context>(*this, m_swapChain.Get(), m_context.Get(), CreateRenderTargetView(*m_swapChain.Get()));
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
        BE_DX_CALL(m_device->CreateBuffer(&bufferDescriptor, nullptr, &bufferPtr));

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
            BE_DX_CALL(m_device->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));
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
            
            BE_DX_CALL(m_device->CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &layoutPtr));
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
        BE_DX_CALL(m_device->CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));

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
        BE_DX_CALL(swapChain.GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
        BE_DX_CALL(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

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
} // namespace be::graphics::d3d11
