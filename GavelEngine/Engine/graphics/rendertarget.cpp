#include "rendertarget.h"
#include <iostream>
RenderTarget::RenderTarget(int width,int height,DXGI_FORMAT render_buffer_target_format)
{

	this->buffer_format = render_buffer_target_format;
	this->width = width;
	this->height = height;
}


RenderTarget::~RenderTarget()
{
}

void RenderTarget::init(Graphics& gfx)
{
	D3D11_VIEWPORT vp;
	D3D11_TEXTURE2D_DESC texture_desc = {};
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc = {};
	vp.Width = (float)width;
	vp.Height =(float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gfx.context->RSSetViewports(1, &vp);
	//setting a render target texture	
	texture_desc.Width = width;
	texture_desc.Height = width;
	texture_desc.ArraySize = 1;
	texture_desc.MipLevels = 1;
	texture_desc.Format = buffer_format;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET) | D3D11_BIND_SHADER_RESOURCE;
	texture_desc.MiscFlags = 0;
	texture_desc.CPUAccessFlags = 0;
	throw_if_fail(gfx.device->CreateTexture2D(&texture_desc, NULL, &render_target_texture));
	//modifying it to fit a depth buffer texture requirement
	texture_desc.Format = DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
	texture_desc.BindFlags &= ~D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
	texture_desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	throw_if_fail(gfx.device->CreateTexture2D(&texture_desc, NULL, &depth_target_texture));
	//the depth stencil view	
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_view_desc = {};
	depth_view_desc.Format = DXGI_FORMAT::DXGI_FORMAT_D16_UNORM;
	depth_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_view_desc.Texture2D.MipSlice = 0;
	throw_if_fail(gfx.device->CreateDepthStencilView(depth_target_texture.Get(), &depth_view_desc, &depth_stencil_view));
	//render target view 	
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc = {};
	render_target_view_desc.Format = buffer_format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	render_target_view_desc.Texture2D.MipSlice = 0;
	throw_if_fail(gfx.device->CreateRenderTargetView(render_target_texture.Get(), &render_target_view_desc, &render_target));
	

	//depth texture resource view
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Format = DXGI_FORMAT_R16_UNORM;
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	shader_resource_view_desc.Texture2D.MipLevels = 1;
	throw_if_fail(gfx.device->CreateShaderResourceView(depth_target_texture.Get(), &shader_resource_view_desc, &depth_target_srv));


	//render target texture resource view
	shader_resource_view_desc.Format = buffer_format;
	throw_if_fail(gfx.device->CreateShaderResourceView(render_target_texture.Get(), &shader_resource_view_desc, &render_target_srv));


}

void RenderTarget::bind(Graphics& gfx)
{	
	adjust_view_port(gfx);
	const float clear_color[] = { 0.0,1.0,0.0,1.0 };
	gfx.context->ClearRenderTargetView(render_target.Get(), clear_color);
	gfx.context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	gfx.context->OMSetRenderTargets(1u, render_target.GetAddressOf(), depth_stencil_view.Get());
		
}

void RenderTarget::bind_as_texture(Graphics& gfx, RT_Type rt_type, int slot)
{
	switch (rt_type)
	{
		case RT_Type::DEPTH:
			gfx.context->PSSetShaderResources(slot, 1, depth_target_srv.GetAddressOf());
			break;
		case RT_Type::RENDER_TARGET:
			gfx.context->PSSetShaderResources(slot, 1, render_target_srv.GetAddressOf());
			break;
	}
}


void RenderTarget::adjust_view_port(Graphics& gfx)
{
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	gfx.context->RSSetViewports(1, &vp);

}
