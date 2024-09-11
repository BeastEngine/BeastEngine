#pragma once
#include "TestSetup.h"

#include <Beast/Graphics/Camera2D.h>

namespace be::tests::unit
{
    class Camera2DTest : public testing::Test
    {
    protected:
        void AssertMatrixEqual(const Mat4& expected, const Mat4& actual) const
        {
            // first row
            EXPECT_FLOAT_EQ(expected[0][0], actual[0][0]);
            EXPECT_FLOAT_EQ(expected[0][1], actual[0][1]);
            EXPECT_FLOAT_EQ(expected[0][2], actual[0][2]);
            EXPECT_FLOAT_EQ(expected[0][3], actual[0][3]);

            // second row
            EXPECT_FLOAT_EQ(expected[1][0], actual[1][0]);
            EXPECT_FLOAT_EQ(expected[1][1], actual[1][1]);
            EXPECT_FLOAT_EQ(expected[1][2], actual[1][2]);
            EXPECT_FLOAT_EQ(expected[1][3], actual[1][3]);

            // third row
            EXPECT_FLOAT_EQ(expected[2][0], actual[2][0]);
            EXPECT_FLOAT_EQ(expected[2][1], actual[2][1]);
            EXPECT_FLOAT_EQ(expected[2][2], actual[2][2]);
            EXPECT_FLOAT_EQ(expected[2][3], actual[2][3]);

            // fourth row
            EXPECT_FLOAT_EQ(expected[3][0], actual[3][0]);
            EXPECT_FLOAT_EQ(expected[3][1], actual[3][1]);
            EXPECT_FLOAT_EQ(expected[3][2], actual[3][2]);
            EXPECT_FLOAT_EQ(expected[3][3], actual[3][3]);
        }
    };

    TEST_F(Camera2DTest, GetViewMatrixWillCreateCorrectViewMatrix)
    {
        const graphics::Viewport viewport = {{800.0f, 600.0f}};
        const Vec2 position = viewport.dimensions / 2.0f;

        const Mat4 expectedViewMatrix = {
            0.0025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0033333333f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f};

        graphics::Camera2D sut(viewport, position);
        
        const auto actualViewMatrix = sut.GetViewMatrix();
        AssertMatrixEqual(expectedViewMatrix, actualViewMatrix);
    }

    TEST_F(Camera2DTest, GetViewMatrixWillCreateCorrectViewMatrixWithTranslation)
    {
        const graphics::Viewport viewport = {{800.0f, 600.0f}};
        const Vec2 position = {500.0f, 535.0f};

        // after transforming by translationVector = { -100.0f,  -235.0f, 0.0f };
        const Mat4 expectedViewMatrix = {
            0.0025f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0033333333f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.25f, -1.783333333f, 0.0f, 1.0f};

        graphics::Camera2D sut(viewport, position);

        const auto actualViewMatrix = sut.GetViewMatrix();
        AssertMatrixEqual(expectedViewMatrix, actualViewMatrix);
    }

    TEST_F(Camera2DTest, GetViewMatrixWillCreateCorrectViewMatrixWithScale)
    {
        const graphics::Viewport viewport = {{800.0f, 600.0f}};
        const Vec2 position = viewport.dimensions / 2.0f;
        const float scale = -1.25;

        const Mat4 expectedViewMatrix = {
            -0.003125f, 0.0f, 0.0f, 0.0f, 0.0f, -0.0041666666f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.25f, 1.25f, 0.0f, 1.0f};

        graphics::Camera2D sut(viewport, position);
        sut.Zoom(scale);

        const auto actualViewMatrix = sut.GetViewMatrix();
        AssertMatrixEqual(expectedViewMatrix, actualViewMatrix);
    }
} // namespace be::tests::unit