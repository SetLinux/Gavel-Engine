#pragma once
#define NOMINMAX
#include "../utils/utils.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
class RenderTarget;
class Graphics {
public:
	Graphics(HWND hwnd);
	~Graphics();
	void end_frame() const;
	void clear_buffer(float red, float green, float blue) const;
	void set_render_target() const;
	Microsoft::WRL::ComPtr<ID3D11Device> device{ nullptr };
	Microsoft::WRL::ComPtr <IDXGISwapChain> swap{ nullptr };
	Microsoft::WRL::ComPtr <ID3D11DeviceContext>context{ nullptr };
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> target{ nullptr };
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> depth_stencil_view;
	double width{ 0 };
	double height{ 0 };
};