#include "Engine/io/window.h"
#include "Engine/graphics/graphics.h"
#include "Engine/graphics/ibo.h"
#include "Engine/graphics/vbo.h"
#include "Engine/graphics/shader.h"
#include "Engine/systems/render_system.h"
int CALLBACK WinMain(
	HINSTANCE h_instance,
	HINSTANCE h_prev_instance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Window window("Gavel Engine", 1024, 720, h_instance);
	Graphics gfx(window.hwnd);
	entt::registry reg;
	auto entity = reg.create();
	reg.assign<Transform>(entity);
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
	base_vertex_shader.init(gfx);
	base_vertex_shader.bind(gfx);
	PixelShader base_pixel_shader("base_pixel_shader.cso");
	base_pixel_shader.init(gfx);
	base_pixel_shader.bind(gfx);
	while (!window.window_should_close)
	{
		window.message_pump();
		gfx.clear_buffer(0.0, 1.0, 0.0);
		gfx.set_render_target();
//		vbor.bind(gfx);
//		ibo.bind(gfx);
		rs.render(reg, gfx);
		gfx.end_frame();



	}

}