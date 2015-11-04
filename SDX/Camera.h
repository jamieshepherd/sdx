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

            void MoveForward();
            void MoveBackward();
            void MoveLeft();
            void MoveRight();
            void TurnLeft();
            void TurnRight();

            void Shutdown();

            float      g_Speed;

        protected:
            XMFLOAT3   m_Position;
            XMFLOAT3   m_Target;
            XMFLOAT3   m_Up;

            float m_RotationX = 0.0f;
            float m_RotationY = 0.0f;
            float m_RotationZ = 0.0f;

            XMFLOAT4X4 m_ViewMatrix;
    };
}