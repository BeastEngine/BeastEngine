#pragma once
#include "Beast/Assertions.h"
#include "Beast/PlatformSetup.h"

#include <source_location>
#include <exception>
#include <format>

#include <comdef.h>
#include <filesystem>
#include <iostream>

namespace be::graphics::d3d11
{
    constexpr inline void Assert([[maybe_unused]] HRESULT result, [[maybe_unused]] const std::source_location location = std::source_location::current())
    {
        BE_ASSERT_MSG(SUCCEEDED(result), "D3D11 call failed with result {}!\n{}::{}::{}", result, location.file_name(), location.function_name(), location.line());
    }

    inline void CheckResult([[maybe_unused]] HRESULT result, [[maybe_unused]] const std::source_location location = std::source_location::current())
    {
        if (FAILED(result))
        {
            _com_error err(result);
            LPCTSTR errMsg = err.ErrorMessage();

            std::filesystem::path p(errMsg);

            Assert(result, location);
            throw std::runtime_error(std::format("D3D11 call failed with result {}!\n{}::{}::{}", p.string(), location.file_name(), location.function_name(), location.line()));
        }
    }
} // namespace be::graphics::d3d11
