#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <functional>
#include "../../Engine/utils/utils.h"
#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

class Window
{
public:
	Window( std::string window_name,int width, int height,HINSTANCE hinstance);
	void message_pump();
	~Window();
	bool window_should_close{ false };
	HWND hwnd;
	void add_binding(char c, std::function<void()> func);
	std::function<void(float dx,float dy)> mouse_function;
	std::vector<char> raw_input_buffer;
	//cursor management
	void confine_cursor();
	void free_curosr();
	int width;
	int height;
private:

	std::unordered_map<char, std::function<void()>> key_bindings;
	static LRESULT CALLBACK WndProc(HWND h_wnd, UINT msg, WPARAM w_param, LPARAM l_param);
	MSG msg_;
};


