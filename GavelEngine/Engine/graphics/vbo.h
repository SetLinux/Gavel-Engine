#pragma once
#include <DirectXMath.h>
#include <vector>
#include "graphics.h"
template<typename T>
class VBO{
public:
	VBO() = default;
	~VBO() = default;
	void init(Graphics& gfx) override {
		D3D11_BUFFER_DESC bDesc = { 0 };
		bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bDesc.Usage = D3D11_USAGE_DEFAULT;
		bDesc.CPUAccessFlags = 0u;
		bDesc.MiscFlags = 0u;
		bDesc.ByteWidth = sizeof(T) * vertices.size();
		bDesc.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd = { 0 };
		sd.pSysMem = vertices.data();

		throw_if_fail(gfx.device->CreateBuffer(&bDesc, &sd, &vbo));
	}
	void bind(Graphics& gfx) override {
		const UINT stride = sizeof(T);
		const UINT offset = 0;
		gfx.context->IASetVertexBuffers(0, 1, vbo.GetAddressOf(), &stride, &offset);
	}
	std::vector<T> vertices;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vbo{ nullptr };
};