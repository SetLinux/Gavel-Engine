#include "shader.h"
#include <iostream>
VertexShader::VertexShader(std::string shader_path, std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc) : shader_path_(shader_path) , input_element_desc(input_element_desc)
{

}

void VertexShader::init(Graphics& gfx)
{
	std::wstring widepath;
	widepath.assign(shader_path_.begin(), shader_path_.end());
	throw_if_fail(D3DReadFileToBlob(widepath.c_str(), &blob));
	throw_if_fail(gfx.device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertex_shader));
	throw_if_fail(gfx.device->CreateInputLayout(&input_element_desc[0], input_element_desc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &input_layout));
	
}

void VertexShader::bind(Graphics& gfx)
{
	gfx.context->VSSetShader(vertex_shader.Get(), 0, 0);
	gfx.context->IASetInputLayout(input_layout.Get());
}



PixelShader::PixelShader(std::string shader_path) : shader_path_(shader_path)
{

}

void PixelShader::init(Graphics& gfx)
{
	std::wstring widepath;
	widepath.assign(shader_path_.begin(), shader_path_.end());
	throw_if_fail((D3DReadFileToBlob(widepath.c_str(), &blob)));
	throw_if_fail(gfx.device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixel_shader));
}

void PixelShader::bind(Graphics& gfx)
{
	gfx.context->PSSetShader(pixel_shader.Get(), 0, 0);
}
