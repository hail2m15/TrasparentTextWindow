#include "TransparentWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    TransparentWindow window(hInstance);

    if (!window.Create()) {
        //MessageBox(NULL, L"Failed to create window", L"Error", MB_ICONERROR);
        return 1;
    }

    window.Show(nCmdShow);
    window.DisplayText(L"Transparent Window Ready!");
    SetTimer(NULL, 1, 2000, NULL); // Could use class method if needed
    Sleep(2000);
    window.DisplayText(L"suhail");
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
