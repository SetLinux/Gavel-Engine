#pragma once
#include <Windows.h>
#include <iostream>
#include <comdef.h>
#include <system_error>
inline void throw_if_fail(HRESULT hr) 
{	
	HRESULT hres = hr;	
	if (FAILED(hres)) {	
		std::string message = std::system_category().message(hres);	
		std::cout << message << std::endl;
	}	
}	

