#pragma once
#include "../utils/utils.h"
#include "graphics.h"
enum class RT_Type : int
{
	DEPTH = 0,
	RENDER_TARGET = 1
};
class RenderTarget 
{
public:
	RenderTarget(int width,int height,DXGI_FORMAT render_buffer_target_format);
	~RenderTarget();
	void init(Graphics& gfx);
	void bind(Graphics& gfx);
	void bind_as_texture(Graphics& gfx, RT_Type  rt_type, int slot = 0);
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  render_target;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  render_target_srv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  depth_target_srv;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>  render_target_texture;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>  depth_target_texture;
	void adjust_view_port(Graphics& gfx);
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;
	int width;
	int height;
	DXGI_FORMAT buffer_format;
};

