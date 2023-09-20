#pragma once
#include <Beast/Assertions.h>

namespace be::graphics::d3d11
{
#ifndef BE_D3D11_CHECK
    #define BE_D3D11_CHECK(hresult) BE_CRITICAL_ASSERT(SUCCEEDED(hresult))
#endif

} // namespace be::graphics::d3d11
