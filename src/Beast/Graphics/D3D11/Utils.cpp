#include "Beast/Graphics/D3D11/Utils.hpp"
#include "Beast/Graphics/D3D11/D3D11.hpp"
#include "Beast/Graphics/D3D11/Buffer.hpp"

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

    API CreateAPI(be::WindowHandle hwnd)
    {
        wrl::ComPtr<ID3D11Device> device = nullptr;
        wrl::ComPtr<IDXGISwapChain> swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> context = nullptr;

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
            &swapChain,
            &device,
            nullptr,
            &context
        );
        BE_DX_CALL(result);

        return {std::move(device), std::move(swapChain), std::move(context)};
    }

    RenderTarget CreateRenderTarget(API& api)
    {
        wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;

        wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
        BE_DX_CALL(api.SwapChain().GetBuffer(0u, __uuidof(ID3D11Resource), &backBuffer));
        BE_DX_CALL(api.Device().CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

        return RenderTarget{std::move(renderTargetView)};
    }

    VertexBuffer CreateVertexBuffer(API& api, uint32 stride, uint32 maxSize)
    {
        D3D11_BUFFER_DESC bufferDescriptor = {
            .ByteWidth = maxSize,
            .Usage = D3D11_USAGE_DYNAMIC,
            .BindFlags = D3D11_BIND_VERTEX_BUFFER,
            .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
            .MiscFlags = 0,
            .StructureByteStride = stride,
        };
        wrl::ComPtr<ID3D11Buffer> buffer;
        BE_DX_CALL(api.Device().CreateBuffer(&bufferDescriptor, nullptr, &buffer));

        return {std::move(buffer), stride, maxSize};
    }

    VertexShader CreateVertexShader(API& api, const FilesystemPath& filepath, const InputLayout& inputLayout)
    {
        wrl::ComPtr<ID3D11VertexShader> shaderPtr;
        wrl::ComPtr<ID3D11InputLayout> layoutPtr;
        wrl::ComPtr<ID3DBlob> shaderByteCode = LoadShaderByteCode(filepath);

        auto& device = api.Device();

        // Create Shader
        {
            BE_DX_CALL(device.CreateVertexShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));
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

            BE_DX_CALL(device.CreateInputLayout(inputElements.data(), static_cast<UINT>(inputElements.size()), shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), &layoutPtr));
        }

        return {std::move(shaderPtr), std::move(layoutPtr)};
    }

    PixelShader CreatePixelShader(API& api, const FilesystemPath& filepath)
    {
        wrl::ComPtr<ID3D11PixelShader> shaderPtr;
        wrl::ComPtr<ID3DBlob> shaderByteCode = LoadShaderByteCode(filepath);
        BE_DX_CALL(api.Device().CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));

        return PixelShader{std::move(shaderPtr)};
    }
} // namespace be::graphics::d3d11
