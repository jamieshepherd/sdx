#include "Shared.h"

namespace SDX
{
    //--------------------------------------------------------------------------------------
    // void ThrowIfFailed()
    // If the hresult for an operation returns failed, throw an error and message box
    //--------------------------------------------------------------------------------------
    void ThrowIfFailed(HRESULT hr, LPWSTR message)
    {
        if (FAILED(hr)) {
            MessageBox(NULL, message, L"Critical error", MB_ICONWARNING | MB_OK);
            PostQuitMessage(0);
        }
    }
}