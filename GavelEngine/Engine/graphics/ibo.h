#pragma once
#include <vector>
#include "graphics.h"
class IBO {
public:
	IBO() = default;
	~IBO() = default;
	void init(Graphics& gfx) ;
	void bind(Graphics& gfx) ;
	std::vector<unsigned int> indices;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> ibo{ nullptr };
	bool inited = false;
};