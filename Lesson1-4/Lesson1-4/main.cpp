// include the basic windows header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Constants.h"
#include "Direct3D.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")


MyDirect3D *myd3d;

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	int X = GetSystemMetrics(SM_CXSCREEN);
	int Y = GetSystemMetrics(SM_CYSCREEN);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		"Snowman for Netease",    // title of the window
		//WS_EX_TOPMOST | WS_POPUP,    // window style
		WS_OVERLAPPEDWINDOW,
		(X - SCREEN_WIDTH) / 2,    // x-position of the window
		(Y - SCREEN_HEIGHT) / 2,    // y-position of the window
		SCREEN_WIDTH,    // width of the window
		SCREEN_HEIGHT,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		hInstance,    // application handle
		NULL);    // used with multiple windows, NULL

				  // display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	myd3d = new MyDirect3D(hWnd);
	GameTime::init();

	freopen("log.txt", "w", stdout);
	// enter the main loop:
	printf("Game start\n");

	// this struct holds Windows event messages
	MSG msg;

	// Enter the infinite message loop
	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		// If the message is WM_QUIT, exit the while loop
		if (msg.message == WM_QUIT)
			break;

		GameTime::update();
		// Run game code here
		myd3d->renderFrame();
	}

	delete myd3d;

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}

	case WM_LBUTTONDOWN:
	{
		myd3d->setMousing(true);
	}
	break;

	case WM_LBUTTONUP:
	{
		//myd3d->setMousing(false);
	}
	break;

	case WM_SETCURSOR:
	{
		SetCursor(NULL);
		myd3d->ShowCursor(false);
		return TRUE;
	}
	break;
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
