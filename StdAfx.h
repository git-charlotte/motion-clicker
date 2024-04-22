#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <dwmapi.h>
#include <map>

#include <string> 
#include <vector>

#include <iostream>
#include <algorithm>

#pragma warning(disable: 4005) // Disable warning C4005 (macro redefinition)
#include <d3d11.h>
#include <d3dx11.h>
#pragma warning(default: 4005) // Re-enable warning C4005

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")

#include "Thirdparty/Dear ImGui/backends/imgui_impl_dx11.h"
#include "Thirdparty/Dear ImGui/backends/imgui_impl_win32.h"
#include "Thirdparty/Dear ImGui/imgui.h"
#include "Thirdparty/Dear ImGui/imgui_internal.h"
#include "Thirdparty/Dear ImGui/addons/imgui_addons.h"
#include "Thirdparty/Dear ImGui/addons/imgui_notification.h"
#include "Thirdparty/Dear ImGui/misc/imgui_freetype.h"

#include "DirectX.h"
#include "Window.h"
#include "Gui.h"
#include "Textures.h"
#include "Globals.h"
#include "algorithm.h"
