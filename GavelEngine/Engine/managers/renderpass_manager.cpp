#include "renderpass_manager.h"

RenderPassManager::RenderPassManager()
{
}

RenderPassManager::~RenderPassManager()
{
}

std::shared_ptr<RenderTarget> RenderPassManager::create_render_target(Graphics& gfx,int id, int width, int height, DXGI_FORMAT buffer_format)
{
	render_targets_map[id] = std::make_shared<RenderTarget>(width, height, buffer_format);
	render_targets_map[id]->init(gfx);
	return render_targets_map[id];
}

void RenderPassManager::bind_render_target(Graphics& gfx,int id)
{
	render_targets_map[id]->bind(gfx);
}

void RenderPassManager::bind_render_target_as_texture(Graphics& gfx, int id, int slot,RT_Type rt_type)
{
#ifdef _DEBUG
	//BOOST_ASSERT_MSG(current_bound_render_target != id, "Can't bind a rendertarget as SRV while it's bound to be drawn into");
#endif
	render_targets_map[id]->bind_as_texture(gfx, rt_type, slot);
}
