#pragma once

#include "Shared.h"

namespace SDX
{
    class Camera
    {
        public:
            Camera();
            Camera(XMFLOAT3 position, XMFLOAT3 target);
            virtual ~Camera();

            void SetPositions(XMFLOAT3 position, XMFLOAT3 target);
            XMMATRIX GetViewMatrix();
            void Shutdown();

        protected:
            XMFLOAT3 m_Position;
            XMFLOAT3 m_Target;
            XMFLOAT3 m_Up;
    };
}