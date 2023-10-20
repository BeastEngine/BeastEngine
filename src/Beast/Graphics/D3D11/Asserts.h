#pragma once
#include "Beast/Assertions.h"
#include "Beast/PlatformSetup.h"

#include <source_location>
#include <exception>
#include <format>

namespace be::graphics::d3d11
{
    constexpr inline void Assert([[maybe_unused]] HRESULT result, [[maybe_unused]] const std::source_location location = std::source_location::current())
    {
        BE_ASSERT_MSG(SUCCEEDED(result), "D3D11 call failed with result {}!\n{}::{}::{}", result, location.file_name(), location.function_name(), location.line());
    }

    constexpr inline void CheckResult([[maybe_unused]] HRESULT result, [[maybe_unused]] const std::source_location location = std::source_location::current())
    {
        if (FAILED(result))
        {
            Assert(result, location);
            throw std::runtime_error(std::format("D3D11 call failed with result {}!\n{}::{}::{}", result, location.file_name(), location.function_name(), location.line()));
        }
    }
} // namespace be::graphics::d3d11
