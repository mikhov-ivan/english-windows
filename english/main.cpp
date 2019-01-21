#include "Globals.hpp"
#include "MainWindow.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE PrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MainWindow mainWindow;

	if (!mainWindow.Create(hInstance, L"Improve Yourself: English Vocabulary")) {
		return E_FAIL;
	}

	ShowWindow(mainWindow.Window(), nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int) msg.wParam;
}