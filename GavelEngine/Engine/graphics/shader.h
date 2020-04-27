#pragma once
#include <string>
#include "graphics.h"
#include <vector>
class VertexShader  {
public:
	VertexShader(std::string shader_path, std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc);
	void init(Graphics& gfx);
	void bind(Graphics& gfx);
private:
	std::string shader_path_;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
};

class PixelShader  {
public:
	PixelShader(std::string shader_path);
	void init(Graphics& gfx);
	void bind(Graphics& gfx);
private:
	std::string shader_path_;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

};