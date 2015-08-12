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
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
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

	RegisterClass(&wc);

	RECT desktopRect = RECT();
	GetWindowRect(GetDesktopWindow(), &desktopRect);
	int width = (int)desktopRect.right;
	int height = (int)desktopRect.bottom;

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"ThingRenderer",    // Window text

		// Size and position
		//WS_POPUP,
		WS_POPUP | WS_BORDER,
#if defined(_DEBUG)
		100, 100, width - 200, height - 200,
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
	HRESULT hr = app.Initialise(hInstance, hwnd, width, height);
	if (FAILED(hr))
		return hr;

	// Run the message loop.

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (app.HandleInput(msg) == -1)
				break;
			app.Update();
			app.Draw();
		}
	}

	app.Release();

	return 0;
}
