#ifndef BASE_WINDOW_HPP
#define BASE_WINDOW_HPP

#include <Windows.h>
#include <commctrl.h>

#include "Resource.hpp"

template< typename DERIVED_TYPE >
class BaseWindow {
public:
	BaseWindow() {
		m_hwnd = NULL;
	}

	bool Create(
		HINSTANCE hInstance,
		PCWSTR lpWindowName,
		DWORD dwExStyle = 0,
		DWORD dwStyle = CS_HREDRAW | CS_VREDRAW | WS_OVERLAPPEDWINDOW,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = DEF_WINDOW_WIDTH,
		int nHeight = DEF_WINDOW_HEIGHT,
		HWND hWndParent = 0,
		HMENU hMenu = 0) {

		WNDCLASSEX wcex;
		
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = DERIVED_TYPE::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hIcon = NULL;
		wcex.hCursor = NULL;
		wcex.hbrBackground = CreatePatternBrush(LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1)));
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = ClassName();
		wcex.hIconSm = NULL;

		RegisterClassEx(&wcex);
		HWND hWnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

		return (hWnd != NULL ? true : false);
	}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DERIVED_TYPE* pThis = NULL;
		if (uMsg == WM_CREATE) {
			CREATESTRUCT* pCreateStruct = reinterpret_cast< CREATESTRUCT* > (lParam);
			pThis = reinterpret_cast< DERIVED_TYPE* > (pCreateStruct->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pThis);
			pThis->m_hwnd = hWnd;
			pThis->Init(hWnd);
		} else {
			pThis = (DERIVED_TYPE*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
		}

		if (pThis != NULL) {
			return pThis->HandleMessage(uMsg, wParam, lParam);
		} else {
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}

	HWND Window() const {
		return m_hwnd;
	}

protected:
	virtual PCWSTR ClassName() const = 0;
	virtual void Init(HWND hWnd) = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};

#endif