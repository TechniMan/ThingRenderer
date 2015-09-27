#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include "GraphicsApp.h"

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

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	int argc = __argc;
	char ** argv = __argv;

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"ThingRenderer Window Class";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassW(&wc);

	RECT desktopRect = RECT();
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	int width = (int)desktopRect.right;
	int height = (int)desktopRect.bottom;
	int nCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
	if (nCaptionHeight == 0)
		Error((HRESULT)GetLastError());

	// Create the window.

	HWND hwnd = CreateWindowExW(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"ThingRenderer",    // Window text

		// Size and position
		//WS_OVERLAPPEDWINDOW,
		WS_POPUP | WS_BORDER,
#if defined(_DEBUG)
		100, 100 - nCaptionHeight, width - 200, height - 200 + nCaptionHeight,
#else
		0, 0, width, height,
#endif
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	GraphicsApp app = GraphicsApp();
	HRESULT hr = app.Initialise(hInstance, hwnd, width - 200, height - 200 + nCaptionHeight);
	if (FAILED(hr))
		return hr;

	// Run the message loop.

	MSG msg = {0};
	while (WM_QUIT != msg.message)
	{
		//GetMessageW(&msg, NULL, 0, 0); // Causes program to run slowly
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			int a = app.HandleInput(msg);
			if (a == -1)
			{ // Escape
				break;
			}
			else if (a == 0)
			{ // Not handled by app
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			app.Update();
			app.Draw();
		}
	}

	app.Release();

	return 0;
}
