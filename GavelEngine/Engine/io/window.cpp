#include "window.h"
#include <iostream>
#include "hidusage.h"



Window::Window(std::string window_name,  int width, int height, HINSTANCE hinstance) : msg_({ 0 })
{
	this->width = width;
	this->height = height;
	// Get console window:
	FILE* console_output;
	FILE* console_error;
	if (AllocConsole()) {
		freopen_s(&console_output, "CONOUT$", "w", stdout);
		freopen_s(&console_error, "CONERR$", "w", stderr);
	}
	const auto class_name = L"DX11 Engine";
	//register window class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = class_name;
	wc.hIconSm = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.lpfnWndProc = WndProc;
	RegisterClassEx(&wc);
	//create window instance
	HWND h_wnd = CreateWindowEx(0, class_name, L"TEST",
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU,
		200, 200,
		width, height,
		NULL, NULL, hinstance, NULL);
	SetWindowLongPtr(h_wnd, GWLP_USERDATA, (LONG_PTR)this);
	this->hwnd = h_wnd;
	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);
	RAWINPUTDEVICE rid[1];
	rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rid[0].dwFlags = RIDEV_INPUTSINK;
	rid[0].hwndTarget = h_wnd;
	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));
	//confine_cursor();
	throw_if_fail(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
}
void Window::confine_cursor()
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	MapWindowPoints(hwnd, nullptr, (POINT*)& rect, 2);
	ClipCursor(&rect);
}
void Window::free_curosr()
{
	ShowCursor(true);
	ClipCursor(nullptr);
}
void Window::message_pump()
{
	ZeroMemory(&msg_, sizeof(MSG));
	while (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE)) {

		TranslateMessage(&msg_);
		DispatchMessage(&msg_);
		if (msg_.message == WM_QUIT) {
			window_should_close = true;
		}
	}
}

Window::~Window()
{
}

void Window::add_binding(char c, std::function<void()> func)
{
	this->key_bindings[c] = func;
}

LRESULT Window::WndProc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param)
{

	Window* current = reinterpret_cast<Window*>((GetWindowLongPtr(h_wnd, GWLP_USERDATA)));
	switch (msg){
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			if(current->key_bindings.find((char)w_param) != current->key_bindings.end())
			current->key_bindings[(char)w_param]();
			break;
		case WM_INPUT: {
			UINT size = 0;
			GetRawInputData((HRAWINPUT)(l_param), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
			if (size > 0)
			{
				current->raw_input_buffer.resize(size);
				GetRawInputData((HRAWINPUT)(l_param), RID_INPUT, current->raw_input_buffer.data(), &size, sizeof(RAWINPUTHEADER));
				auto& ri = (RAWINPUT&)(*current->raw_input_buffer.data());
				if (ri.header.dwType == RIM_TYPEMOUSE) {
					
					current->mouse_function((float)ri.data.mouse.lLastX, (float)ri.data.mouse.lLastY);
				}
			}
			break;
		}

		case WM_DPICHANGED:

				//const int dpi = HIWORD(wParam);
				//printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
				const RECT* suggested_rect = (RECT*)l_param;
				::SetWindowPos(h_wnd, NULL, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
			break;
	}
	return DefWindowProc(h_wnd,msg,w_param,l_param);
}
