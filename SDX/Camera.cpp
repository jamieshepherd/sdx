#include "Camera.h"
#include <iostream>
#include <string>

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // Camera()
    // Camera constructor
    //--------------------------------------------------------------------------------------
    Camera::Camera() :
        m_Position(XMFLOAT3(0.0f, 5.0f, -8.0f)), m_Target(XMFLOAT3(0.0f, 0.0f, 1.0f)), m_Up(XMFLOAT3(0.0f, 1.0f, 0.0f)),
        g_Speed(0.005f), g_Sensitivity(0.005f),
        m_CamYaw(0.0f), m_CamPitch(0.0f)
    {
    }

    //--------------------------------------------------------------------------------------
    // Camera()
    // Camera destructor
    //--------------------------------------------------------------------------------------
    Camera::~Camera()
    {
    }

    //--------------------------------------------------------------------------------------
    // void SetPositions()
    // Set the camera position and target
    //--------------------------------------------------------------------------------------
    void Camera::SetPositions(XMFLOAT3 position, XMFLOAT3 target)
    {
        m_Position = position;
        m_Target = target;
    }

    //--------------------------------------------------------------------------------------
    // XMFLOAT4X4 GetViewMatrix()
    // Get the view matrix of our camera
    //--------------------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------------------
    // void MoveForward()
    // Move the camera position forward
    //--------------------------------------------------------------------------------------
    void Camera::MoveForward()
    {
        m_Position.z += g_Speed;
    }

    //--------------------------------------------------------------------------------------
    // void MoveBackward()
    // Move the camera position backward
    //--------------------------------------------------------------------------------------
    void Camera::MoveBackward()
    {
        m_Position.z -= g_Speed;
    }

    //--------------------------------------------------------------------------------------
    // void MoveLeft()
    // Move the camera position left
    //--------------------------------------------------------------------------------------
    void Camera::MoveLeft()
    {
        m_Position.x -= g_Speed;
    }

    //--------------------------------------------------------------------------------------
    // void MoveRight()
    // Move the camera position right
    //--------------------------------------------------------------------------------------
    void Camera::MoveRight()
    {
        m_Position.x += g_Speed;
    }

    //--------------------------------------------------------------------------------------
    // void UpdateMouseXY()
    // Update the Yaw and Pitch based on our mouse position
    //--------------------------------------------------------------------------------------
    void Camera::UpdateMouseXY(long xPosition, long yPosition)
    {
        m_CamYaw += xPosition * g_Sensitivity;
        m_CamPitch += yPosition * g_Sensitivity;
    }

    //--------------------------------------------------------------------------------------
    // void SetCameraMode()
    // Change between free-look and 3rd person
    //--------------------------------------------------------------------------------------
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

    //--------------------------------------------------------------------------------------
    // void Shutdown()
    // Release all objects and shutdown
    //--------------------------------------------------------------------------------------
    void Camera::Shutdown()
    {
    }
}