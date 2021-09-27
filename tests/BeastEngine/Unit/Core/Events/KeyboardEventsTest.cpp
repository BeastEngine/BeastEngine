#include "KeyboardEventsTest.h"
#include <BeastEngine/Core/Events/KeyboardEvents.h>
#include <BeastEngine/Core/Input/InputCodes.h>

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

    TEST_F(KeyboardEventsTest, KeyPressedEventConstructorWillProperltyInitializeParent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_PRESSED;

        const auto sut = KeyPressedEvent(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }

    TEST_F(KeyboardEventsTest, KeyHeldDownEventConstructorWillProperltyInitializeParent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_HELD_DOWN;

        const auto sut = KeyHeldDownEvent(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }

    TEST_F(KeyboardEventsTest, KeyReleasedEventConstructorWillProperltyInitializeParent)
    {
        const auto expectedKeyCode = KeyCode::INVALID;
        const auto expectedEventType = KeyboardEventType::EVENT_KEY_RELEASED;

        const auto sut = KeyReleasedEvent(expectedKeyCode);
        ASSERT_EQ(expectedKeyCode, sut.GetKey());
        ASSERT_EQ(expectedEventType, sut.GetType());
    }
} // namespace be::tests::unit
