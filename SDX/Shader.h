#pragma once

#include "Shared.h"

namespace SDX
{
    class Shader
    {
        public:
            Shader();
            virtual ~Shader();
            void LoadShaders();
    };
}