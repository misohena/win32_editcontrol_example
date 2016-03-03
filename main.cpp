#include <windows.h>
#include "resource.h"

void FitWindowToParent(HWND hChild, HWND hParent)
{
	RECT r;
	GetClientRect(hParent, &r);
	SetWindowPos(hChild, NULL, 0, 0, r.right, r.bottom, SWP_NOZORDER);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		{
			char text[4096];
			DWORD textBytes = 0;
			HANDLE file = CreateFile(L"example.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if(file != INVALID_HANDLE_VALUE){
				ReadFile(file, text, sizeof(text), &textBytes, NULL);
			}


			WCHAR wtext[(sizeof(text)+1)*sizeof(WCHAR)];
			MultiByteToWideChar(CP_UTF8, 0, text, textBytes, wtext, sizeof(wtext) / sizeof(wtext[0]) - 1);
			wtext[textBytes] = '\0';

			RECT rect;
			GetClientRect(hwnd, &rect);
			CreateWindow(TEXT("EDIT"), wtext,
				WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
				0, 0, rect.right, rect.bottom, hwnd, NULL, ((LPCREATESTRUCT)(lparam))->hInstance, NULL);
			return 0;
		}
	case WM_SIZING:
		FitWindowToParent(GetWindow(hwnd, GW_CHILD), hwnd);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_HREDRAW|CS_VREDRAW, WndProc, 0, 0, hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP))};
	wc.lpszClassName = L"MainWindow";
	RegisterClassEx(&wc);

	HWND hwnd = CreateWindow(L"MainWindow", L"MainWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
