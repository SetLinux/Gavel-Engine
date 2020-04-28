#pragma once
#include "../graphics/rendertarget.h"
#include <unordered_map>
#include <boost/assert.hpp>
class RenderPassManager
{
public:
	RenderPassManager();
	~RenderPassManager();

	std::shared_ptr<RenderTarget> create_render_target(Graphics& gfx,int id,int width,int height,DXGI_FORMAT buffer_format);
	void bind_render_target(Graphics& gfx,int id);
	void bind_render_target_as_texture(Graphics& gfx, int id, int slot,RT_Type rt_type);
private:
	std::unordered_map<int, std::shared_ptr<RenderTarget>> render_targets_map;
	int current_bound_render_target{ -1 };
};

