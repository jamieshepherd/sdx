#include "Shared.h"

namespace SDX
{
    void ThrowIfFailed(HRESULT hr, LPWSTR message)
    {
        if (FAILED(hr)) {
            MessageBox(NULL, message, L"Critical error", MB_ICONWARNING | MB_OK);
            PostQuitMessage(0);
        }
    }
}