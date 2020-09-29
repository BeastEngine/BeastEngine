#include "MouseEventsTest.h"

#include <BeastEngine/Core/Events/MouseEvents.h>
#include <BeastEngine/Core/Input/InputCodes.h>

namespace be::tests::unit
{
    TEST_F(MouseEventsTest, MouseEventConstructorWillProperlyInitializeWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(0, 0);
        const auto expectedScrollValue = 0;
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseEvent(expectedType);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseEventConstructorWillProperlyInitializeWithPassedValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(10, 0);
        const auto expectedScrollValue = 32;
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseEvent(expectedType, expectedCoordinates, expectedScrollValue, expectedMouseButton);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseMovedEventConstructorWillProperlyInitializeParentWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(5, 10);

        const auto expectedScrollValue = 0;
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseMovedEvent(expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseScrolledEventConstructorWillProperlyInitializeParentWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_SCROLLED;
        const auto expectedScrollValue = 412;
        const auto expectedCoordinates = IntVec2(5, 10);

        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseScrolledEvent(expectedScrollValue, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseButtonPressedEventConstructorWillProperlyInitializeParentWithDefaultValues)
    {
        const auto expectedMouseButton = MouseButtonCode::BUTTON_LEFT;
        const auto expectedCoordinates = IntVec2(532, 131);
        const auto expectedType = MouseEventType::EVENT_MOUSE_BUTTON_PRESSED;

        const auto expectedScrollValue = 0;

        const auto sut = MouseButtonPressedEvent(expectedMouseButton, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseButtonHeldEventConstructorWillProperlyInitializeParentWithDefeaultValues)
    {
        const auto expectedMouseButton = MouseButtonCode::BUTTON_LEFT;
        const auto expectedType = MouseEventType::EVENT_MOUSE_BUTTON_HELD_DOWN;

        const auto expectedScrollValue = 0;

        const auto sut = MouseButtonHeldDownEvent(expectedMouseButton);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseButtonReleasedEventConstructorWillProperlyInitializeParentWithDefaultValues)
    {
        const auto expectedMouseButton = MouseButtonCode::BUTTON_LEFT;
        const auto expectedCoordinates = IntVec2(532, 131);
        const auto expectedType = MouseEventType::EVENT_MOUSE_BUTTON_RELEASED;

        const auto expectedScrollValue = 0;

        const auto sut = MouseButtonReleasedEvent(expectedMouseButton, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValue, sut.GetScrollValue());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }
} // namespace be::tests::unit
