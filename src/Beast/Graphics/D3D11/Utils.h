#pragma once
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/Buffer.h"
#include "Beast/Graphics/D3D11/Shader.h"
#include "Beast/Graphics/D3D11/Textures.h"
#include "Beast/Graphics/D3D11/RenderTarget.h"

#include "Beast/Graphics/Types.h"

#include "Beast/Common/Filesystem/Types.h"

namespace be::graphics
{
    struct Image;
}

namespace be::graphics::d3d11
{
    API CreateAPI(be::WindowHandle hwnd);
    RenderTarget CreateRenderTarget(API& api);

    VertexBuffer CreateVertexBuffer(API& api, uint32 stride, uint32 maxSize);
    ConstantBuffer CreateConstantBuffer(API& api, uint32 size);
    VertexShader CreateVertexShader(API& api, const fs::Path& filepath, const InputLayout& inputLayout);
    PixelShader CreatePixelShader(API& api, const fs::Path& filepath);
    Texture CreateTexture(API& api, const Image& textureData);
} // namespace be::graphics::d3d11
