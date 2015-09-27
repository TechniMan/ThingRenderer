#include "Error.h"

void Error(HRESULT hr)
{
	MessageBox(NULL, _com_error(hr).ErrorMessage(), L"D3D11 Error", MB_ICONSTOP);
}

void Error(HWND hw, HRESULT hr)
{
	MessageBox(hw, _com_error(hr).ErrorMessage(), L"D3D11 Error", MB_ICONSTOP);
}
