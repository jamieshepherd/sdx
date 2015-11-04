#pragma once

#include <memory>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include <DirectXPackedVector.h>
#include <D3Dcompiler.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "WICTextureLoader.h"

#define ReleaseObject(object) if((object) != nullptr) { object->Release(); object = nullptr; }
#define KEYDOWN( name, key ) ( name[key] & 0x80 )

using namespace DirectX;

struct VERTEX
{
    XMFLOAT3 pos;
    //XMFLOAT4 color;
    XMFLOAT2 tex;
};

struct cbPerObject
{
    XMMATRIX  WVP;
};

namespace SDX
{
    void ThrowIfFailed(HRESULT hr, LPWSTR message);
}