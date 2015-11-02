#include "Camera.h"

namespace SDX
{
    Camera::Camera() :
        m_Position(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        m_Target(XMFLOAT3(0.0f, 0.0f, 0.0f)),
        m_Up(XMFLOAT3(0.0f, 1.0f, 0.0f))
    {
    }

    Camera::Camera(XMFLOAT3 position, XMFLOAT3 target) :
        m_Position(position),
        m_Target(target),
        m_Up(XMFLOAT3(0.0f, 1.0f, 0.0f))
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::SetPositions(XMFLOAT3 position, XMFLOAT3 target)
    {
        m_Position = position;
        m_Target = target;
    }

    XMFLOAT4X4 Camera::GetViewMatrix()
    {
        XMVECTOR EyePosition = XMLoadFloat3(&m_Position);
        XMVECTOR FocusTarget = XMLoadFloat3(&m_Target);
        XMVECTOR Up = XMLoadFloat3(&m_Up);
        XMMATRIX ViewMatrix = XMMatrixLookAtLH(EyePosition, FocusTarget, Up);
        XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);
        
        return m_ViewMatrix;
    }

    void Camera::MoveForward()
    {
        m_Position.x += 0.1f;
    }

    void Camera::MoveBackward()
    {
        m_Position.x -= 0.1f;
    }

    void Camera::MoveLeft()
    {
    }

    void Camera::MoveRight()
    {
    }

    void Camera::Shutdown()
    {
    }
}