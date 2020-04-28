#include "Engine/io/window.h"
#include "Engine/graphics/graphics.h"
#include "Engine/managers/renderpass_manager.h"
#include "Engine/graphics/ibo.h"
#include "Engine/graphics/vbo.h"
#include "Engine/graphics/shader.h"
#include "Engine/systems/render_system.h"
#define IS_DOWN(x)  (GetKeyState(x) & 0x8000 )

enum render_targets : int
{
	COLOR  = 0,
	DEPTH = 1
};
int CALLBACK WinMain(
	HINSTANCE h_instance,
	HINSTANCE h_prev_instance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	float yaw = 00.0f;
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	
	Window window("Gavel Engine", 1024, 720, h_instance);
	window.mouse_function = [&](float dx, float dy) {
			float sensitivty = 0.3f;
			float xoffset, yoffset;
			xoffset = dx * sensitivty;
			yoffset = -dy * sensitivty;
			yaw -= xoffset;
			pitch += yoffset;
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		
	};
	Graphics gfx(window.hwnd);
	camera_desc cameradesc;
	cameradesc.camera_type = CameraType::Projeciton;
	cameradesc.perspective.aspect_ratio = gfx.width / gfx.height;
	cameradesc.near_z = 0.01f;
	cameradesc.far_z = 100.0f;
	cameradesc.perspective.fov = 0.4f * 3.14f;
	Camera main_cameras(cameradesc);
	
	//Camera camera()
//	main_cameras.set_position();
	DirectX::XMVECTOR CameraPosition = {0.0,0.0,-1.0f};

	VBO<Vertex> quad_vbo;
	quad_vbo.vertices =
	{
		{{1.0f, 1.0f, 0.f,1.f}},
		{{1.0f, -1.0f, 0.f,1.f}},
		{{-1.0f, -1.0f, 0.f,1.f}},
		{{-1.0f, 1.0f, 0.f,1.f}},

	};
	quad_vbo.init(gfx);
	IBO quad_ibo;
	quad_ibo.indices =
	{
		0, 1, 2,
		2, 3, 0
	};
	quad_ibo.init(gfx);
	entt::registry reg;
	auto entity = reg.create();
	reg.assign<Transform>(entity);
	reg.get<Transform>(entity).scale = DirectX::XMVECTOR{ 10.3f,10.3f,10.3f };
	reg.assign<Renderable>(entity);
	reg.get<Renderable>(entity).vbo.vertices =
	{
		{{1.0f, 1.0f, 0.f,1.f}},
		{{1.0f, -1.0f, 0.f,1.f}},
		{{-1.0f, -1.0f, 0.f,1.f}},
		{{-1.0f, 1.0f, 0.f,1.f}},
	};
	reg.get<Renderable>(entity).vbo.init(gfx);
	reg.get<Renderable>(entity).ibo.indices =
	{
		0, 1, 2,
		2, 3, 0 
	};
	reg.get<Renderable>(entity).ibo.init(gfx);
	VertexShader base_vertex_shader("base_vertex_shader.cso", 
		{
			{"Position",0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		}
	);
	RenderSystem rs;
	rs.init(gfx, main_cameras);
	base_vertex_shader.init(gfx);
	base_vertex_shader.bind(gfx);
	PixelShader base_pixel_shader("base_pixel_shader.cso");
	base_pixel_shader.init(gfx);
	base_pixel_shader.bind(gfx);
	RenderPassManager rpm;
	rpm.create_render_target(gfx, render_targets::COLOR, gfx.width, gfx.height, DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
	while (!window.window_should_close)
	{
		main_cameras.set_position(CameraPosition);
		main_cameras.set_orientation(yaw, pitch, 0);
		DirectX::XMVECTOR direction{ DirectX::XMScalarCos(DirectX::XMConvertToRadians(yaw)) * DirectX::XMScalarCos(DirectX::XMConvertToRadians(pitch)),DirectX::XMScalarSin(DirectX::XMConvertToRadians(pitch)) ,DirectX::XMScalarSin(DirectX::XMConvertToRadians(yaw)) * DirectX::XMScalarCos(DirectX::XMConvertToRadians(pitch))  , 0.0 };

		if (IS_DOWN((int)'W')) {
			CameraPosition = DirectX::XMVectorAdd(DirectX::XMVectorScale(direction, 1.05f), CameraPosition);
		}
		if (IS_DOWN((int)'S')) {
			CameraPosition = DirectX::XMVectorAdd(DirectX::XMVectorScale(direction, -1.05f), CameraPosition);
		}
		rpm.bind_render_target(gfx, render_targets::COLOR);
		rs.render(reg, gfx,main_cameras);
		window.message_pump();
		gfx.clear_buffer(0.0, 1.0, 0.0);
		gfx.set_render_target();
		//rpm.bind_render_target_as_texture(gfx, render_targets::COLOR, 0, RT_Type::RENDER_TARGET);
		gfx.end_frame();
	}

}