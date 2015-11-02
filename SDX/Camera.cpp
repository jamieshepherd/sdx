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
        XMVECTOR CamPosition = XMLoadFloat3(&m_Position);
        XMVECTOR CamTarget = XMLoadFloat3(&m_Target);
        XMVECTOR CamUp = XMLoadFloat3(&m_Up);
        XMMATRIX ViewMatrix = XMMatrixLookAtLH(CamPosition, CamTarget, CamUp);
        XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);
        
        return m_ViewMatrix;
    }

    void Camera::MoveForward()
    {
        m_Position.x += 0.01f;
    }

    void Camera::MoveBackward()
    {
        m_Position.x -= 0.01f;
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