#include "Beast/Graphics/Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace be::graphics
{
    Camera2D::Camera2D(const Viewport& viewport, const Vec2& position) noexcept
        : m_viewport(viewport), m_position(position)
    {
    }

    void Camera2D::SetViewport(const Viewport& viewport) noexcept
    {
        m_viewport = viewport;
    }

    void Camera2D::SetPosition(const Vec2& position) noexcept
    {
        m_position = position;
    }

    void Camera2D::Move(const Vec2& vector) noexcept
    {
        m_position += vector;
    }

    void Camera2D::Zoom(float newZoom) noexcept
    {
        m_zoom = newZoom;
    }

    Mat4 Camera2D::GetViewMatrix() const noexcept
    {
        const Mat4 orthoMatrix = glm::ortho(0.0f, m_viewport.dimensions.x, 0.0f, m_viewport.dimensions.y, 1.0f, -1.0f);
        const glm::vec2 translatedPosition = -m_position + (m_viewport.dimensions * 0.5f);

        Mat4 viewMatrix = glm::translate(orthoMatrix, {translatedPosition.x, translatedPosition.y, 0.0f});
        return glm::scale(glm::mat4(1.0f), {m_zoom, m_zoom, 1.0f}) * viewMatrix;
    }

    const Viewport& Camera2D::GetViewport() const noexcept
    {
        return m_viewport;
    }

    const Vec2& Camera2D::GetPosition() const noexcept
    {
        return m_position;
    }
} // namespace be::graphics
