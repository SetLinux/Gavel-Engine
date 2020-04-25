#pragma once
#include <Windows.h>
#include <comdef.h>

inline void throw_if_fail(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw _com_error(hr);
	}
}
