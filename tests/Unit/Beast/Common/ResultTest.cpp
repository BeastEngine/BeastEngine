#include <Beast/Common/Result.h>
#include <Beast/Common/Types.h>

#include <gtest/gtest.h>
#include <memory>

namespace be::tests::unit
{
    class ResultTest : public testing::Test
    {
    };

    TEST_F(ResultTest, ResultSuccessfulWhenCopyableValueGiven)
    {
        const int expectedValue = 10;
        Result<int> actualResult(expectedValue);

        ASSERT_TRUE(actualResult);
        ASSERT_EQ(expectedValue, actualResult.Value());
    }

    TEST_F(ResultTest, ResultSuccessfulWhenMovableValueGiven)
    {
        int expectedValue = 10;

        Unique<int> expectedValuePtr = MakeUnique<int>(10);
        Result<Unique<int>> actualResult(std::move(expectedValuePtr));

        ASSERT_TRUE(actualResult);
        ASSERT_EQ(expectedValue, *actualResult.Value());
    }

    TEST_F(ResultTest, ResultUnsuccessfulWhenErrorGiven)
    {
        const std::string expectedError = "This is an error string";
        Result<int> actualErrorResult(ResultError{.message = expectedError});

        ASSERT_FALSE(actualErrorResult);
        ASSERT_EQ(expectedError, actualErrorResult.Error().message);
    }

    TEST_F(ResultTest, ResultUnsuccessfulWhenStringCopyGiven)
    {
        const std::string expectedError = "This is an error string";
        Result<int> actualErrorResult(expectedError);

        ASSERT_FALSE(actualErrorResult);
        ASSERT_EQ(expectedError, actualErrorResult.Error().message);
    }

    TEST_F(ResultTest, ResultUnsuccessfulWhenMovingStringIntoResult)
    {
        Result<int> actualErrorResult("This is an error string");

        ASSERT_FALSE(actualErrorResult);
        ASSERT_EQ("This is an error string", actualErrorResult.Error().message);
    }

    TEST_F(ResultTest, TakeValueWillMoveTheValueFromResult)
    {
        int expectedValue = 10;

        Unique<int> expectedValuePtr = MakeUnique<int>(10);
        Result<Unique<int>> actualResult(std::move(expectedValuePtr));

        Unique<int> actualValuePtr = actualResult.TakeValue();
        ASSERT_EQ(expectedValue, *actualValuePtr);
        ASSERT_EQ(actualResult.Value(), nullptr);
    }

    TEST_F(ResultTest, ArrowOperatorAllowsAccessingValueMembersDirectly)
    {
        struct MyResultValue
        {
            int value1 = 0;
            int value2 = 1;
        };

        Result<MyResultValue> sut{MyResultValue{}};
        ASSERT_EQ(0, sut->value1);
        ASSERT_EQ(1, sut->value2);
    }

    TEST_F(ResultTest, AccessorsWillThrowIfValueOrErrorNotPresent)
    {
        struct MyResultValue
        {
            int value1 = 0;
            int value2 = 1;
        };

        Result<MyResultValue> valueResult{MyResultValue{}};
        Result<MyResultValue> errorResult{ResultError{.message = "Error"}};

        ASSERT_THROW([[maybe_unused]] auto&& _ = valueResult.Error(), std::bad_variant_access);
        ASSERT_THROW([[maybe_unused]] auto&& _ = errorResult->value1, std::bad_variant_access);
        ASSERT_THROW([[maybe_unused]] auto&& _ = errorResult.Value(), std::bad_variant_access);
        ASSERT_THROW([[maybe_unused]] auto&& _ = errorResult.TakeValue(), std::bad_variant_access);
    }

    // This should not compile
    // TEST_F(ResultTest, CompileTimeConceptsCheck)
    // {
    //     Result<std::string> notValid{"Value"};
    //     Result<std::string_view> notValid{"Value"};
    //     Result<ResultError> notValid{ResultError{"Value"}};
    //     Result<const char*> notValid{"asdf"};
    // }
} // namespace be::tests::unit