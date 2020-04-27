#pragma once
#include <entt/entt.hpp>
#include "../components/components.h"
#include "../graphics/graphics.h"
class RenderSystem
{
public:

	RenderSystem() = default;
	~RenderSystem() = default;
	void render(entt::registry& reg, Graphics& gfx)
	{
		reg.group<Transform, Renderable>().each([&](entt::entity ent, Transform& trans, Renderable& rend) {
			rend.vbo.bind(gfx);
			rend.ibo.bind(gfx);
			gfx.context->DrawIndexed(rend.ibo.indices.size(), 0, 0 );
		});
	};
};
