#include "graphics.h"
#include <iostream>
Graphics::Graphics(HWND hwnd) {
	namespace wrl = Microsoft::WRL;
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	this->width = clientRect.right;
	this->height = clientRect.bottom;
	
	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferDesc.Width = clientRect.right;
	sd.BufferDesc.Height = clientRect.bottom;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	
	sd.Flags = 0;
	throw_if_fail( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swap,
		&device,
		nullptr,
		&context
	));
	wrl::ComPtr<ID3D11Resource> back_buffer{ nullptr };
	swap->GetBuffer(0, __uuidof(ID3D11Resource), &back_buffer);
	device->CreateRenderTargetView(back_buffer.Get(), nullptr, &target);
	// Create the depth buffer for use with the depth/stencil view.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0; 
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	depthStencilBufferDesc.Width = clientRect.right;
	depthStencilBufferDesc.Height = clientRect.bottom;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	throw_if_fail(device->CreateTexture2D(&depthStencilBufferDesc, nullptr, &depth_stencil_buffer));
	throw_if_fail(device->CreateDepthStencilView(depth_stencil_buffer.Get(), nullptr, &depth_stencil_view));
	context->OMSetRenderTargets(1u, target.GetAddressOf(), depth_stencil_view.Get());

	D3D11_VIEWPORT vp;
	vp.Width = (float)clientRect.right;
	vp.Height = (float)clientRect.bottom;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Graphics::~Graphics()
{
}

void Graphics::end_frame() const
{
	throw_if_fail(swap->Present(1, 0));
}

void Graphics::clear_buffer(float red, float green, float blue) const
{
	const float color[] = { red,green,blue };
	context->ClearRenderTargetView(target.Get(), color);
	context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void Graphics::set_render_target() const
{
	context->OMSetRenderTargets(1u, target.GetAddressOf(), depth_stencil_view.Get());
}
