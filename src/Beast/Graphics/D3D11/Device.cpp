#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Device.h"

#include "Beast/Common/UUIDGenerator.h"

#include <d3dcompiler.h>

namespace be::graphics::d3d11
{
    Device::Device(const IWindow& window)
    {
        const auto hwnd = window.GetHandle();
        const auto& windowDimensions = window.GetDimensions();

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = windowDimensions.x;
        sd.BufferDesc.Height = windowDimensions.y;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM ;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        const auto result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_context
        );
        CheckResult(result);
    }

    wrl::ComPtr<ID3D11DeviceContext> Device::GetContext() const noexcept
    {
        return m_context;
    }

    wrl::ComPtr<IDXGISwapChain> Device::GetSwapChain() const noexcept
    {
        return m_swapChain;
    }

    wrl::ComPtr<ID3D11RenderTargetView> Device::CreateRenderTargetView(IDXGISwapChain& swapChain) const
    {
        wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;

        wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
        CheckResult(swapChain.GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
        CheckResult(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

        return renderTargetView;
    }

    graphics::VertexBuffer Device::CreateVertexBuffer()
    {
        graphics::VertexBuffer buffer{.id = GenerateUUID4()};
        m_vertexBuffers.insert({buffer.id, {*m_device.Get()}});

        return buffer;
    }

    graphics::VertexShader Device::CreateVertexShader(const FilesystemPath& filepath, const InputLayout& inputLayout)
    {
        wrl::ComPtr<ID3DBlob> shaderByteCode;
        CheckResult(D3DReadFileToBlob(filepath.wstring().c_str(), &shaderByteCode));

        graphics::VertexShader shader{.id = GenerateUUID4()};
        m_vertexShaders.insert({shader.id, {*m_device.Get(), *shaderByteCode.Get(), inputLayout}});

        return shader;
    }

    graphics::PixelShader Device::CreatePixelShader(const FilesystemPath& filepath)
    {
        wrl::ComPtr<ID3DBlob> shaderByteCode;
        CheckResult(D3DReadFileToBlob(filepath.wstring().c_str(), &shaderByteCode));

        graphics::PixelShader shader{.id = GenerateUUID4()};
        m_pixelShaders.insert({shader.id, {*m_device.Get(), *shaderByteCode.Get()}});

        return shader;
    }

    void Device::Run()
    {
        static const auto vertexBuffer = CreateVertexBuffer();
        static const std::vector<Vertex> vertices = {
            {
                .position = {-0.5f, -0.5f},
                .color = {1.0f, 0.0f, 0.0f, 1.0f},
            },
            {
                .position = {0.0f, 0.5f},
                .color = {0.0f, 1.0f, 0.0f, 1.0f},
            },
            {
                .position = {0.5f, -0.5f},
                .color = {0.0f, 0.0f, 1.0f, 1.0f},
            },
        };
        auto& buffer = m_vertexBuffers.at(vertexBuffer.id);
        buffer.Update(*m_context.Get(), vertices);
        buffer.Bind(*m_context.Get());

        /*const float color[] = {0.0f, 0.0f, 0.0f, 1.0f};

        static const auto renderTargetView = CreateRenderTargetView(*m_swapChain.Get());
        m_context->ClearRenderTargetView(renderTargetView.Get(), color);*/

        //static const std::vector<Vertex> vertices = {
        //    {
        //        .position = {0.5f, -0.5f},
        //        //.color = {1.0f, 0.0f, 0.0f, 1.0f},
        //    },
        //    {
        //        .position = {-0.5f, -0.5f},
        //        //.color = {0.0f, 1.0f, 0.0f, 1.0f},
        //    },
        //    {
        //        .position = {0.0f, 0.5f},
        //        //.color = {0.0f, 0.0f, 1.0f, 1.0f},
        //    },
        //};

        //wrl::ComPtr<ID3D11Buffer> vertexBuffer;

        //D3D11_BUFFER_DESC bufferDescriptor = {};
        //bufferDescriptor.ByteWidth = sizeof(Vertex) * 3; // This must be the MAX size of the buffer
        //bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
        //bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        //bufferDescriptor.CPUAccessFlags = 0;
        //bufferDescriptor.MiscFlags = 0;
        //bufferDescriptor.StructureByteStride = sizeof(Vertex);

        //D3D11_SUBRESOURCE_DATA bufferData = {};
        //bufferData.pSysMem = vertices.data();

        //CheckResult(m_device->CreateBuffer(&bufferDescriptor, &bufferData, &vertexBuffer));
        //const UINT stride = sizeof(Vertex);
        //const UINT offset = 0u;
        //m_context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

        //// VERTEX SHADER
        //wrl::ComPtr<ID3D11VertexShader> vShader;
        //wrl::ComPtr<ID3DBlob> shaderByteCode;
        //CheckResult(D3DReadFileToBlob(L"VertexShader.cso", &shaderByteCode));

        //CheckResult(m_device->CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &vShader));
        //m_context->VSSetShader(vShader.Get(), nullptr, 0);

        //D3D11_INPUT_ELEMENT_DESC decs{
        //    .SemanticName = "POSITION",
        //    .SemanticIndex = 0,
        //    .Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT,
        //    .InputSlot = 0,
        //    .InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
        //    .InstanceDataStepRate = 0,
        //};

        //wrl::ComPtr<ID3D11InputLayout> inputLayout;
        //CheckResult(m_device->CreateInputLayout(&decs, 1u, shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &inputLayout));
        //m_context->IASetInputLayout(inputLayout.Get());

        //// PIXEL SHADER
        //wrl::ComPtr<ID3D11PixelShader> pShader;
        //CheckResult(D3DReadFileToBlob(L"PixelShader.cso", &shaderByteCode));

        //CheckResult(m_device->CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &pShader));
        //m_context->PSSetShader(pShader.Get(), nullptr, 0u);

        //// RENDER TARGET
        ////m_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        /*D3D11_VIEWPORT d3dViewport{};
        d3dViewport.Width = 800.0f;
        d3dViewport.Height = 600.0f;
        d3dViewport.MinDepth = 0.0f;
        d3dViewport.MaxDepth = 1.0f;
        d3dViewport.TopLeftX = 0.0f;
        d3dViewport.TopLeftY = 0.0f;
        m_context->RSSetViewports(1, &d3dViewport);

        m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->Draw(3u, 0u);*/
        
        static const InputLayout layout{
            .vertexAttributes = {
                {"POSITION", InputLayout::VertexAttribute::Format::Vec2},
                {"COLOR", InputLayout::VertexAttribute::Format::Vec4},
            },
        };
        static const auto vertexShaderRef = CreateVertexShader("VertexShader.cso", layout);
        auto& vertexShader = m_vertexShaders.at(vertexShaderRef.id);
        vertexShader.Use(*m_context.Get());

        static const auto pixelShaderRef = CreatePixelShader("PixelShader.cso");
        auto& pixelShader = m_pixelShaders.at(pixelShaderRef.id);
        pixelShader.Use(*m_context.Get());

        D3D11_VIEWPORT d3dViewport{};
        d3dViewport.Width = 800.0f;
        d3dViewport.Height = 600.0f;
        d3dViewport.MinDepth = 0.0f;
        d3dViewport.MaxDepth = 1.0f;
        d3dViewport.TopLeftX = 0.0f;
        d3dViewport.TopLeftY = 0.0f;
        m_context->RSSetViewports(1, &d3dViewport);

        /*static const auto renderTargetView = CreateRenderTargetView(*m_swapChain.Get());

        m_context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), m_view.Get());
        Color color{0.0f, 0.0f, 0.0f, 1.0f};
        m_context->ClearRenderTargetView(renderTargetView.Get(), color.Data());*/

        m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_context->Draw(3, 0);
    }
} // namespace be::graphics::d3d11
