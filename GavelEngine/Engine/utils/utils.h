#pragma once
#include <Windows.h>
#include <iostream>
#include <comdef.h>
#include <system_error>
inline void throw_if_fail(HRESULT hr)
{
	//	assert(!FAILED(hr) && hr);
	
	if (FAILED(hr)) {
		std::string message = std::system_category().message(hr);
		std::cout << message << "\n";
	}
}

