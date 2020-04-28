#pragma once
#include <DirectXMath.h>
#include "../graphics/vbo.h"
#include "../graphics/ibo.h"
namespace dx = DirectX;
struct Vertex
{
	dx::XMVECTOR position;
};
struct Transform 
{
	dx::XMVECTOR position;
	dx::XMVECTOR scale;
	bool is_dirty{ true };
	dx::XMMATRIX model;
	dx::XMMATRIX model_inversed;
};
struct Renderable
{
	VBO<Vertex> vbo;
	IBO ibo;
};
