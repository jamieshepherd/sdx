#pragma once

#include "Shared.h"

namespace SDX
{
    class Camera
    {
        public:
            Camera();
            virtual ~Camera();

            void SetPositions(XMFLOAT3 position, XMFLOAT3 target);
            XMFLOAT4X4 GetViewMatrix();
            int        m_CamMode = 1;
            float      g_Speed;
            float      g_Sensitivity;

            void MoveForward();
            void MoveBackward();
            void MoveLeft();
            void MoveRight();
            void UpdateMouseXY(long xPosition, long yPosition);
            void SetCameraMode(int mode);

            void Shutdown();
        protected:
            XMFLOAT4X4 m_ViewMatrix;
            XMFLOAT3   m_Position;
            XMFLOAT3   m_Target;
            XMFLOAT3   m_Up;
            float      m_CamYaw;
            float      m_CamPitch;          

            XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    };
}