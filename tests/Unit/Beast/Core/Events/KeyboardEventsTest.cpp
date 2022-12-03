#include <Unit/Beast/Core/Events/KeyboardEventsTest.h>

#include <Beast/Core/Events/KeyboardEvents.h>
#include <Beast/Core/Input/InputCodes.h>

namespace be::tests::unit
{
    TEST_F(KeyboardEventsTest, KeyboardEventConstructorWillProperlyInitializeWithGivenValues)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;

        const auto sut = KeyboardEvent(expectedKeyCode, expectedEventType);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }

    TEST_F(KeyboardEventsTest, KeyPressedWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;

        const auto sut = KeyboardEvent::KeyPressed(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }

    TEST_F(KeyboardEventsTest, KeyHeldDownWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_HELD_DOWN;

        const auto sut = KeyboardEvent::KeyHeldDown(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }

    TEST_F(KeyboardEventsTest, KeyReleasedWillCreateCorrectEvent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_RELEASED;

        const auto sut = KeyboardEvent::KeyReleased(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }
} // namespace be::tests::unit
