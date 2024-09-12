#pragma once
#include "Beast/Math/Types.h"
#include "Beast/Graphics/Types.h"

namespace be::graphics
{
    /**
	 * @brief Camera2D class responsible for camera and scene transformations.
	 *
	 * Viewport is the width and height of the visible area and it is static, thus it does not depend
	 * on camera's position or any other value. It's just a part of the screen that is currently visible.
	 * (You can call the viewport a scene).
	 *
	 * Position of the camera is the position inside the game world.
 	 * (No matter where the camera is, the viewport stays the same)
	 */
    class Camera2D
    {
    public:
        Camera2D(const Viewport& viewport, const Vec2& position) noexcept;

        void SetViewport(const Viewport& viewport) noexcept;
        void SetPosition(const Vec2& position) noexcept;
        void Move(const Vec2& vector) noexcept;
        void Zoom(float newZoom) noexcept;

        const Viewport& GetViewport() const noexcept;
        const Vec2& GetPosition() const noexcept;

        /**
		 * @brief Returns orthogonal camera transformation matrix
		 *
		 * @return const Mat4
		 */
        Mat4 GetViewMatrix() const noexcept;

    private:
        /** current camera scale/zoom */
        float m_zoom = 1;

        /** screen width and height */
        Viewport m_viewport;

        /** current camera position */
        Vec2 m_position;
    };
} // namespace be::graphics