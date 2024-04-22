#include <thread>

#include "StdAfx.h"
#include "DirectX.h"
#include "Window.h"
#include "Gui.h"
#include "clicker/clicker.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)clicker::thread::main, nullptr, 0, nullptr);

    Window::CreateWindowAndClass();
    DirectX::Init();
    Textures::Init(DirectX::m_pDevice);
    Window::Setup();
    Gui::Init(Window::hWnd, DirectX::m_pDevice, DirectX::m_pDeviceContext); // Mive into DirectX or use g_Globals
    
    while (Window::bIsRunning)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                Window::bIsRunning = false;
        }

        Gui::Render();
        DirectX::Present();
    }

    Gui::Unload();
    Textures::Unload();
    DirectX::CleanupDeviceD3D();
    Window::Shutdown();

    return 0;
}
