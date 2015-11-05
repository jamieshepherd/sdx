#include "Camera.h"
#include <iostream>
#include <string>

namespace SDX
{
    Camera::Camera() :
        m_Position(XMFLOAT3(0.0f, 5.0f, -8.0f)), m_Target(XMFLOAT3(0.0f, 0.0f, 1.0f)), m_Up(XMFLOAT3(0.0f, 1.0f, 0.0f)),
        g_Speed(0.005f), g_Sensitivity(0.005f),
        m_CamYaw(0.0f), m_CamPitch(0.0f)
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
        XMVECTOR CamUp = XMLoadFloat3(&m_Up);
        XMVECTOR CamPosition = XMLoadFloat3(&m_Position);
        XMVECTOR CamTarget = XMLoadFloat3(&m_Target);

        XMMATRIX CamRotation = XMMatrixRotationRollPitchYaw(m_CamPitch, m_CamYaw, 0);

        if (m_CamMode == 1) {
            CamTarget = XMVector3TransformCoord(DefaultForward, CamRotation);
            CamTarget = XMVector3Normalize(CamTarget);
            CamTarget = CamPosition + CamTarget;
        }
        else if (m_CamMode == 2) {
            CamTarget = XMVector3Normalize(CamTarget);
            CamTarget = CamPosition + CamTarget;
        }
        

        XMMATRIX ViewMatrix = XMMatrixLookAtLH(CamPosition, CamTarget, CamUp);
        XMStoreFloat4x4(&m_ViewMatrix, ViewMatrix);
        
        return m_ViewMatrix;
    }

    void Camera::MoveForward()
    {
        m_Position.z += g_Speed;
    }

    void Camera::MoveBackward()
    {
        m_Position.z -= g_Speed;
    }

    void Camera::MoveLeft()
    {
        m_Position.x -= g_Speed;
    }

    void Camera::MoveRight()
    {
        m_Position.x += g_Speed;
    }

    void Camera::UpdateMouseXY(long xPosition, long yPosition)
    {
        m_CamYaw += xPosition * g_Sensitivity;
        m_CamPitch += yPosition * g_Sensitivity;
    }

    void Camera::SetCameraMode(int mode)
    {
        switch (mode) {
            case 1:
                m_CamMode = 1;
                break;
            case 2:
                m_CamMode = 2;
                break;
            default:
                m_CamMode = 1;
        }
    }

    void Camera::Shutdown()
    {
    }
}