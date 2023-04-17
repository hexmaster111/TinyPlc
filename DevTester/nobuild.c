/******************************************************************************
 * USAGE:
 * install MinGW and pick MinGwBase and include it in your PATH
 * build nobuild.c with the command below
 * gcc -o nobuild .\nobuild.c
 * then you can re-run nobuild, and it will rebuild itself
 ******************************************************************************/

#include <stdio.h>
#define NOBUILD_IMPLEMENTATION
// FROM https://github.com/tsoding/nobuild
#include "nobuild.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect(hwnd, &rc);
        DrawTextA(hdc, L"Hello, World!", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main(int argc, char *argv[])
{
    GO_REBUILD_URSELF(argc, argv);
    INFO("Starting! ");

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Learn to Program Windows",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        wc.hInstance,
        NULL);

    if (hwnd == NULL)
    {
        INFO("CreateWindowEx failed: %d", GetLastError());
    }

    INFO("Window created! ");
    ShowWindow(hwnd, SW_SHOW);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}