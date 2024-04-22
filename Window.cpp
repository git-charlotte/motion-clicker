#include "StdAfx.h"
#include "Window.h"
#include "DirectX.h"

void Window::CreateWindowAndClass()
{
    ImGui_ImplWin32_EnableDpiAwareness();

    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = Window::WindowProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = reinterpret_cast<LPCWSTR>(WINDOW_TITLE);
    wc.lpszClassName = reinterpret_cast<LPCWSTR>("Class01");
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    ::RegisterClassExW(&wc);

    hWnd = ::CreateWindowW(wc.lpszClassName, wc.lpszMenuName, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WindowSize.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (WindowSize.y / 2), WindowSize.x, WindowSize.y, nullptr, nullptr, wc.hInstance, nullptr);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}

void Window::Setup()
{
    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);
}

void Window::Shutdown()
{
    ::DestroyWindow(hWnd);
    ::UnregisterClassA(reinterpret_cast<LPCSTR>(wc.lpszClassName), wc.hInstance);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Forward declare message handler from imgui_impl_win32.cpp
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
        case WM_SIZE:
        {
            if (DirectX::m_pDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                DirectX::CleanupRenderTarget();
                DirectX::m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                DirectX::CreateRenderTarget();
            }
            return 0;
        }
        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        }
        case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            return 0;
        }
    }

    return ::DefWindowProcW(hWnd, message, wParam, lParam);
}