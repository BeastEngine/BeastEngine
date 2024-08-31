#include <Beast/Input/Input.h>

#include <gtest/gtest.h>
#include <iostream>

namespace be::tests::unit
{
    class InputTest : public ::testing::Test
    {
    protected:
    };

    TEST_F(InputTest, KeyPressedTest)
    {
        for (be::uint16 i = 1; i < ToUnderlying(be::KeyCode::SIZE); ++i)
        {
            std::cout << "Running for keycode: " << i << "\n";
            const auto key = be::KeyCode(i);

            be::Input sut{};

            ASSERT_FALSE(sut.IsKeyPressed(key));
            ASSERT_FALSE(sut.IsKeyDown(key));
            ASSERT_FALSE(sut.IsKeyHeldDown(key));

            sut.OnKeyPressed(key);
            ASSERT_TRUE(sut.IsKeyPressed(key));
            ASSERT_TRUE(sut.IsKeyDown(key));
            ASSERT_FALSE(sut.IsKeyHeldDown(key));

            sut.OnKeyHeldDown(key);
            ASSERT_FALSE(sut.IsKeyPressed(key));

            sut.OnKeyReleased(key);
            ASSERT_FALSE(sut.IsKeyPressed(key));
            ASSERT_FALSE(sut.IsKeyDown(key));
            ASSERT_FALSE(sut.IsKeyHeldDown(key));
        }
    }

    TEST_F(InputTest, KeyHeldDownTest)
    {
        for (be::uint16 i = 1; i < ToUnderlying(be::KeyCode::SIZE); ++i)
        {
            std::cout << "Running for keycode: " << i << "\n";
            const auto key = be::KeyCode(i);

            be::Input sut{};

            ASSERT_FALSE(sut.IsKeyPressed(key));
            ASSERT_FALSE(sut.IsKeyDown(key));
            ASSERT_FALSE(sut.IsKeyHeldDown(key));

            sut.OnKeyHeldDown(key);
            ASSERT_FALSE(sut.IsKeyPressed(key));
            ASSERT_TRUE(sut.IsKeyDown(key));
            ASSERT_TRUE(sut.IsKeyHeldDown(key));

            sut.OnKeyReleased(key);
            ASSERT_FALSE(sut.IsKeyPressed(key));
            ASSERT_FALSE(sut.IsKeyDown(key));
            ASSERT_FALSE(sut.IsKeyHeldDown(key));
        }
    }

    TEST_F(InputTest, MouseWheelWillAccumulateSingleMouseUpEvent)
    {
        const auto treshold = 120u;
        be::Input sut{};

        sut.SetMouseWheelScrollThreshold(treshold);
        sut.OnMouseWheelScrolled(40);
        sut.OnMouseWheelScrolled(40);
        sut.OnMouseWheelScrolled(40);

        const auto events = sut.PollMouseWheelEvents();
        ASSERT_EQ(1, events.size());
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_UP, events[0].direction);

        ASSERT_TRUE(sut.PollMouseWheelEvents().empty());
    }

    TEST_F(InputTest, MouseWheelWillAccumulateMultipleMouseUpEvents)
    {
        const auto treshold = 50u;
        be::Input sut{};

        sut.SetMouseWheelScrollThreshold(treshold);
        sut.OnMouseWheelScrolled(50);
        sut.OnMouseWheelScrolled(50);
        sut.OnMouseWheelScrolled(50);

        const auto& events = sut.PollMouseWheelEvents();
        ASSERT_EQ(3, events.size());
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_UP, events[0].direction);
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_UP, events[1].direction);
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_UP, events[2].direction);

        ASSERT_TRUE(sut.PollMouseWheelEvents().empty());
    }

    TEST_F(InputTest, MouseWheelWillAccumulateSingleMouseDownEvent)
    {
        const auto treshold = 120u;
        be::Input sut{};

        sut.SetMouseWheelScrollThreshold(treshold);
        sut.OnMouseWheelScrolled(-40);
        sut.OnMouseWheelScrolled(-40);
        sut.OnMouseWheelScrolled(-40);

        const auto events = sut.PollMouseWheelEvents();
        ASSERT_EQ(1, events.size());
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_DOWN, events[0].direction);

        ASSERT_TRUE(sut.PollMouseWheelEvents().empty());
    }

    TEST_F(InputTest, MouseWheelWillAccumulateMultipleMouseDownEvents)
    {
        const auto treshold = 50u;
        be::Input sut{};

        sut.SetMouseWheelScrollThreshold(treshold);
        sut.OnMouseWheelScrolled(-50);
        sut.OnMouseWheelScrolled(-50);
        sut.OnMouseWheelScrolled(-50);

        const auto events = sut.PollMouseWheelEvents();
        ASSERT_EQ(3, events.size());
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_DOWN, events[0].direction);
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_DOWN, events[1].direction);
        ASSERT_EQ(be::WheelScrollDirection::SCROLL_DOWN, events[2].direction);

        ASSERT_TRUE(sut.PollMouseWheelEvents().empty());
    }

    TEST_F(InputTest, MouseButtonPressedTest)
    {
        for (be::uint16 i = 1; i < ToUnderlying(be::MouseButtonCode::SIZE); ++i)
        {
            std::cout << "Running for mouse button code: " << i << "\n";
            const auto button = be::MouseButtonCode(i);

            be::Vec2i expectedPressedPosition = {10, 5};
            be::Vec2i expectedReleasedPosition = {25, 23};

            be::Input sut{};

            ASSERT_FALSE(sut.IsMouseButtonPressed(button));
            ASSERT_FALSE(sut.IsMouseButtonDown(button));
            ASSERT_FALSE(sut.IsMouseButtonHeldDown(button));

            sut.OnMouseButtonPressed(button, be::Vec2i{expectedPressedPosition});
            ASSERT_TRUE(sut.IsMouseButtonPressed(button));
            ASSERT_TRUE(sut.IsMouseButtonDown(button));
            ASSERT_FALSE(sut.IsMouseButtonHeldDown(button));
            ASSERT_EQ(expectedPressedPosition, sut.GetMousePosition());

            sut.OnMouseButtonHeldDown(button);
            ASSERT_FALSE(sut.IsMouseButtonPressed(button));

            sut.OnMouseButtonReleased(button, be::Vec2i{expectedReleasedPosition});
            ASSERT_FALSE(sut.IsMouseButtonPressed(button));
            ASSERT_FALSE(sut.IsMouseButtonDown(button));
            ASSERT_FALSE(sut.IsMouseButtonHeldDown(button));
            ASSERT_EQ(expectedReleasedPosition, sut.GetMousePosition());
        }
    }

    TEST_F(InputTest, MouseButtonDownTest)
    {
        for (be::uint16 i = 1; i < ToUnderlying(be::MouseButtonCode::SIZE); ++i)
        {
            std::cout << "Running for mouse button code: " << i << "\n";
            const auto button = be::MouseButtonCode(i);

            be::Input sut{};

            ASSERT_FALSE(sut.IsMouseButtonPressed(button));
            ASSERT_FALSE(sut.IsMouseButtonDown(button));
            ASSERT_FALSE(sut.IsMouseButtonHeldDown(button));

            sut.OnMouseButtonHeldDown(button);
            ASSERT_FALSE(sut.IsMouseButtonPressed(button));
            ASSERT_TRUE(sut.IsMouseButtonDown(button));
            ASSERT_TRUE(sut.IsMouseButtonHeldDown(button));

            sut.OnMouseButtonReleased(button, {});
            ASSERT_FALSE(sut.IsMouseButtonPressed(button));
            ASSERT_FALSE(sut.IsMouseButtonDown(button));
            ASSERT_FALSE(sut.IsMouseButtonHeldDown(button));
        }
    }

    TEST_F(InputTest, MouseMoveTest)
    {
        const be::Vec2i expectedPosition = {543, 123};

        be::Input sut{};
        ASSERT_EQ(be::Vec2i(0, 0), sut.GetMousePosition());

        sut.OnMouseMoved(be::Vec2i{expectedPosition});
        ASSERT_EQ(expectedPosition, sut.GetMousePosition());
    }
} // namespace be::tests::unit
