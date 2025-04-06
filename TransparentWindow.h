#pragma once
#include <windows.h>
#include <string>

class TransparentWindow {
public:
    TransparentWindow(HINSTANCE hInstance);
    ~TransparentWindow();

    bool Create(int width = 500, int height = 300);
    void Show(int nCmdShow);
    void DisplayText(const std::wstring& text);

private:
    HWND hwnd;
    HFONT hFont;
    std::wstring displayText;
    HINSTANCE hInstance;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
