#include <Beast/Input/Events/KeyboardEvents.hpp>
#include <Beast/Input/InputCodes.hpp>

#include <gtest/gtest.h>

namespace be::tests::unit
{
    class KeyboardEventsTest : public ::testing::Test
    {
    protected:
    };

    TEST_F(KeyboardEventsTest, KeyboardEventConstructorWillProperlyInitializeWithGivenValues)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;

        const auto sut = KeyboardEvent(expectedKeyCode, expectedEventType);
        ASSERT_EQ(expectedKeyCode, sut.key);
        ASSERT_EQ(expectedEventType, sut.type);
    }

    TEST_F(KeyboardEventsTest, KeyPressedWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;

        const auto sut = KeyboardEvent::KeyPressed(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.key);
        ASSERT_EQ(expectedEventType, sut.type);
    }

    TEST_F(KeyboardEventsTest, KeyHeldDownWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_HELD_DOWN;

        const auto sut = KeyboardEvent::KeyHeldDown(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.key);
        ASSERT_EQ(expectedEventType, sut.type);
    }

    TEST_F(KeyboardEventsTest, KeyReleasedWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_RELEASED;

        const auto sut = KeyboardEvent::KeyReleased(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.key);
        ASSERT_EQ(expectedEventType, sut.type);
    }
} // namespace be::tests::unit
