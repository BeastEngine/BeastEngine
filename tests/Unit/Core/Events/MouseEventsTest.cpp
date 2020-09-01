#include "MouseEventsTest.h"

#include <BeastEngine/Core/Events/MouseEvents.h>
#include <BeastEngine/Core/Input/InputCodes.h>

namespace be::tests::unit
{
    TEST_F(MouseEventsTest, MouseEventConstructorWillProperlyInitializesWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(0, 0);
        const auto expectedScrollValues = IntVec2(0, 0);
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseEvent(expectedType);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseEventConstructorWillProperlyInitializesWithPassedValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(10, 0);
        const auto expectedScrollValues = IntVec2(32, 3);
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseEvent(expectedType, expectedCoordinates, expectedScrollValues, expectedMouseButton);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseMovedEventConstructorWillProperlyInitializesParentWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_MOVED;
        const auto expectedCoordinates = IntVec2(5, 10);

        const auto expectedScrollValues = IntVec2(0, 0);
        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseMovedEvent(expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseScrolledEventConstructorWillProperlyInitializesParentWithDefaultValues)
    {
        const auto expectedType = MouseEventType::EVENT_MOUSE_SCROLLED;
        const auto expectedScrollValues = IntVec2(412, 42);
        const auto expectedCoordinates = IntVec2(5, 10);

        const auto expectedMouseButton = MouseButtonCode::INVALID;

        const auto sut = MouseScrolledEvent(expectedScrollValues, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseButtonPressedEventConstructorWillProperlyInitializesParentWithDefaultValues)
    {
        const auto expectedMouseButton = MouseButtonCode::BUTTON_LEFT;
        const auto expectedCoordinates = IntVec2(532, 131);
        const auto expectedType = MouseEventType::EVENT_MOUSE_BUTTON_PRESSED;

        const auto expectedScrollValues = IntVec2(0, 0);

        const auto sut = MouseButtonPressedEvent(expectedMouseButton, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }

    TEST_F(MouseEventsTest, MouseButtonReleasedEventConstructorWillProperlyInitializesParentWithDefaultValues)
    {
        const auto expectedMouseButton = MouseButtonCode::BUTTON_LEFT;
        const auto expectedCoordinates = IntVec2(532, 131);
        const auto expectedType = MouseEventType::EVENT_MOUSE_BUTTON_RELEASED;

        const auto expectedScrollValues = IntVec2(0, 0);

        const auto sut = MouseButtonReleasedEvent(expectedMouseButton, expectedCoordinates);
        ASSERT_EQ(expectedType, sut.GetType());
        ASSERT_EQ(expectedCoordinates, sut.GetCoordinates());
        ASSERT_EQ(expectedScrollValues, sut.GetScrollValues());
        ASSERT_EQ(expectedMouseButton, sut.GetButton());
    }
} // namespace be::tests::unit
