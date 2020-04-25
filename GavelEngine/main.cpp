#include "Engine/io/window.h"
#include "Engine/graphics/graphics.h"
int CALLBACK WinMain(
	HINSTANCE h_instance,
	HINSTANCE h_prev_instance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	Window window("Gavel Engine", 1024, 720, h_instance);
	Graphics gfx(window.hwnd);
	while (!window.window_should_close)
	{
		window.message_pump();
		gfx.clear_buffer(0.0, 1.0, 0.0);
		gfx.set_render_target();
		gfx.end_frame();
	}

}