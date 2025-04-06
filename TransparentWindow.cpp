#include "TransparentWindow.h"

TransparentWindow::TransparentWindow(HINSTANCE hInst)
    : hwnd(NULL), hFont(NULL), hInstance(hInst) {}

TransparentWindow::~TransparentWindow() {
    if (hFont) {
        DeleteObject(hFont);
    }
}

bool TransparentWindow::Create(int width, int height) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = StaticWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"TransparentWindowClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wc)) return false;

    hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT,
        wc.lpszClassName,
        L"Transparent Window",
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL, hInstance, this);

    return hwnd != NULL;
}

void TransparentWindow::Show(int nCmdShow) {
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

void TransparentWindow::DisplayText(const std::wstring& text) {
    displayText = text;
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// Static window proc for routing messages to the instance method
LRESULT CALLBACK TransparentWindow::StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    TransparentWindow* pThis;
    if (msg == WM_NCCREATE) {
        pThis = static_cast<TransparentWindow*>(((CREATESTRUCT*)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        pThis = (TransparentWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    }

    if (pThis) {
        return pThis->WindowProc(hwnd, msg, wParam, lParam);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT TransparentWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_ERASEBKGND:
        return 1;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdcMem, &rect, hBrush);
        DeleteObject(hBrush);

        SetBkMode(hdcMem, TRANSPARENT);
        SetTextColor(hdcMem, RGB(255, 255, 255));

        if (!hFont) {
            hFont = CreateFont(36, 0, 0, 0, FW_BOLD,
                FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Arial");
        }

        HGDIOBJ oldFont = SelectObject(hdcMem, hFont);
        DrawText(hdcMem, displayText.c_str(), -1, &rect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SelectObject(hdcMem, oldFont);

        POINT ptZero = { 0, 0 };
        SIZE sizeWnd = { rect.right, rect.bottom };
        BLENDFUNCTION blend = { AC_SRC_OVER, 0, 220, 0 };

        UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd,
            hdcMem, &ptZero, RGB(0, 0, 0), &blend, ULW_COLORKEY);

        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_TIMER:
        if (wParam == 1) {
            DisplayText(L"Text updates work perfectly!");
            KillTimer(hwnd, 1);
        }
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
