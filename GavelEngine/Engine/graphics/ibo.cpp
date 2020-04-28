#include "ibo.h"
#include <boost/assert.hpp>
void IBO::init(Graphics& gfx) {
	D3D11_BUFFER_DESC bdesc = { 0 };
	bdesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdesc.Usage = D3D11_USAGE_DEFAULT;
	bdesc.CPUAccessFlags = 0u;
	bdesc.MiscFlags = 0u;
	bdesc.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
	bdesc.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA sd = { 0 };
	sd.pSysMem = indices.data();
	throw_if_fail(gfx.device->CreateBuffer(&bdesc, &sd, &ibo));
	inited = true;
}
void IBO::bind(Graphics& gfx) {
#ifdef _DEBUG
	BOOST_ASSERT_MSG(inited,"Binding an IBO without initializing it");
#endif // DEBUG
	const UINT stride = sizeof(unsigned int);
	const UINT offest = 0;
	gfx.context->IASetIndexBuffer(ibo.Get(), DXGI_FORMAT_R32_UINT, 0);
}