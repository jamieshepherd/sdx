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

    XMMATRIX Camera::GetViewMatrix()
    {
        XMMATRIX ViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_Position), XMLoadFloat3(&m_Target), XMLoadFloat3(&m_Up));
        return ViewMatrix;
    }

    void Camera::Shutdown()
    {
    }
}