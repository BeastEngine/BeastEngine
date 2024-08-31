#pragma once
#include "Beast/PlatformSetup.hpp"
#include "Beast/Debug.hpp"

#include <source_location>
#include <exception>
#include <format>

#include <comdef.h>
#include <filesystem>
#include <iostream>
#include <stdlib.h>

namespace be::graphics::d3d11
{
#define BE_DX_CALL(result)                                                                                  \
    {                                                                                                       \
        if (FAILED(result))                                                                                 \
        {                                                                                                   \
            const auto dx_err_msg = GetErrorMessage(result);                                                \
            BE_ASSERT_MSG(SUCCEEDED(result), "D3D11 call failed with the following error: {}", dx_err_msg); \
            BE_THROW(dx_err_msg);                                                                           \
        }                                                                                                   \
    }

    inline std::string GetErrorMessage(HRESULT result)
    {
        LPWSTR errMessage = nullptr;
        const auto formattingResult = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, result, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&errMessage, 0, NULL);
        BE_WINAPI_CALL(formattingResult);

        std::filesystem::path pth{errMessage};
        const auto requiredStringSize = WideCharToMultiByte(CP_UTF8, 0, errMessage, -1, nullptr, 0, nullptr, nullptr);
        BE_WINAPI_CALL(requiredStringSize);

        std::string errorString;
        errorString.resize(requiredStringSize);

        const auto conversionResult = WideCharToMultiByte(CP_UTF8, 0, errMessage, -1, errorString.data(), requiredStringSize, nullptr, nullptr);
        BE_WINAPI_CALL(conversionResult);

        return errorString;
    }
} // namespace be::graphics::d3d11
