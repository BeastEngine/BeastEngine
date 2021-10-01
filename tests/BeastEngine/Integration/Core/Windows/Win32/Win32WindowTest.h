#pragma once
#ifdef BE_ENABLE_WINDOWS_TESTS
    #include "Win32TestSetup.h"

namespace be::tests::integration
{
    class Win32WindowTest : public Win32Test
    {
    };
} // namespace be::tests::integration
#endif
