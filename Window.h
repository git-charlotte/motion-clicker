#pragma once

namespace Window
{
	void CreateWindowAndClass();
	void Setup();
	void Shutdown();
	LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	inline bool bIsRunning = true;

	inline HWND hWnd;
	inline WNDCLASSEXW wc;
	inline POINTS WindowSize = { 549 , 730 };
}