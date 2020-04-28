#pragma once
#include <iostream>
#include <boost/assert.hpp>
#include "../graphics/graphics.h"
enum class ConstantBufferType : unsigned int
{
	Default = D3D11_USAGE_DEFAULT,
	Dynamic = D3D11_USAGE_DYNAMIC
};
template<typename T>
class ConstantBuffer  {
public:
	ConstantBuffer() {
	}
	~ConstantBuffer() = default;
	void init(Graphics& gfx,ConstantBufferType cbt,int slot,T* t) {
		D3D11_BUFFER_DESC bd = { 0 };
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.Usage = (D3D11_USAGE)cbt;
		bd.CPUAccessFlags = cbt == ConstantBufferType::Dynamic ? D3D11_CPU_ACCESS_WRITE : 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd = { 0 };
		sd.pSysMem = t;
		cbt_ = cbt;
#ifdef _DEBUG
		BOOST_ASSERT_MSG((sizeof(T) % 16) == 0 , "Constant Buffer Struct needs aligment");
#endif
		this->slot_ = slot;
		throw_if_fail(gfx.device->CreateBuffer(&bd, &sd, &cb));
	};
	void bind(Graphics& gfx) 
	{
		gfx.context->VSSetConstantBuffers(slot_, 1, cb.GetAddressOf());
	};
	void bind_frag(Graphics& gfx)
	{
		gfx.context->PSSetConstantBuffers(slot_, 1, cb.GetAddressOf());
	}
	void update(Graphics& gfx,T* t)
	{
		if (cbt_ == ConstantBufferType::Default)
		{
			gfx.context->UpdateSubresource(cb.Get(), 0, 0, t, 0, 0);
		}
		else
		{
			D3D11_MAPPED_SUBRESOURCE mapped_subresource;
			gfx.context->Map(cb.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0,&mapped_subresource);
			memcpy(mapped_subresource.pData, t, sizeof(T));
			gfx.context->Unmap(cb.Get(), 0);
		}
	}
private:
	UINT slot_;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cb{ nullptr };
	ConstantBufferType cbt_;
};