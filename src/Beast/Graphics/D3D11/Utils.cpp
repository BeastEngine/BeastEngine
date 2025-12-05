#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Utils.h"

#include "Beast/Graphics/Images.h"

#include <d3dcompiler.h>

#pragma comment(lib, "DXGI.lib")

namespace be::graphics::d3d11
{
    static wrl::ComPtr<ID3DBlob> LoadShaderByteCode(const fs::Path& shaderFilepath)
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

    ConstantBuffer CreateConstantBuffer(API& api, uint32 size)
    {
        D3D11_BUFFER_DESC bufferDescriptor = {
            .ByteWidth = size,
            .Usage = D3D11_USAGE_DYNAMIC,
            .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
            .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
            .MiscFlags = 0,
            .StructureByteStride = 0,
        };
        wrl::ComPtr<ID3D11Buffer> buffer;
        BE_DX_CALL(api.Device().CreateBuffer(&bufferDescriptor, nullptr, &buffer));

        return {std::move(buffer), size};
    }

    VertexShader CreateVertexShader(API& api, const fs::Path& filepath, const InputLayout& inputLayout)
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

    PixelShader CreatePixelShader(API& api, const fs::Path& filepath)
    {
        wrl::ComPtr<ID3D11PixelShader> shaderPtr;
        wrl::ComPtr<ID3DBlob> shaderByteCode = LoadShaderByteCode(filepath);
        BE_DX_CALL(api.Device().CreatePixelShader(shaderByteCode->GetBufferPointer(), shaderByteCode->GetBufferSize(), nullptr, &shaderPtr));

        return PixelShader{std::move(shaderPtr)};
    }

    Texture CreateTexture(API& api, const Image& textureData)
    {
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = textureData.width;
        textureDesc.Height = textureData.height;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.ArraySize = 1;
        textureDesc.MipLevels = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO: Map number of channels to the proper format
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA resourceData = {0};
        resourceData.pSysMem = textureData.data.data();
        resourceData.SysMemPitch = textureData.width * sizeof(unsigned char) * 4; // TODO: Use number of channels here instead of '4'

        ID3D11Device& device = api.Device();

        wrl::ComPtr<ID3D11Texture2D> texture;
        BE_DX_CALL(device.CreateTexture2D(&textureDesc, &resourceData, &texture));

        wrl::ComPtr<ID3D11ShaderResourceView> textureResourceView;
        D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
        resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceViewDesc.Texture2D.MipLevels = 1;
        resourceViewDesc.Texture2D.MostDetailedMip = 0;
        BE_DX_CALL(device.CreateShaderResourceView(texture.Get(), &resourceViewDesc, &textureResourceView));

        wrl::ComPtr<ID3D11SamplerState> samplerState;
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        BE_DX_CALL(device.CreateSamplerState(&samplerDesc, &samplerState));

        return Texture{std::move(textureResourceView), std::move(samplerState)};
    }
} // namespace be::graphics::d3d11
