#pragma once
#include "Beast/Graphics/D3D11/D3D11.hpp"
#include "Beast/Graphics/D3D11/Buffer.hpp"
#include "Beast/Graphics/D3D11/Shader.hpp"
#include "Beast/Graphics/D3D11/RenderTarget.hpp"

#include "Beast/Graphics/Types.hpp"

#include "Beast/Common/Filesystem/Types.hpp"

namespace be::graphics::d3d11
{
    API CreateAPI(be::WindowHandle hwnd);
    RenderTarget CreateRenderTarget(API& api);

    VertexBuffer CreateVertexBuffer(API& api, uint32 stride, uint32 maxSize);
    VertexShader CreateVertexShader(API& api, const FilesystemPath& filepath, const InputLayout& inputLayout);
    PixelShader CreatePixelShader(API& api, const FilesystemPath& filepath);
} // namespace be::graphics::d3d11
