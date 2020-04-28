#pragma once
#include <entt/entt.hpp>
#include "../components/components.h"
#include "../graphics/graphics.h"
#include "../graphics/constantbuffer.h"
#include "../graphics/camera.h"
class RenderSystem
{
public:

	RenderSystem() = default;
	~RenderSystem() = default;
	void init(Graphics& gfx,Camera& camera)
	{
		proj_struct.projection = camera.projection_matrix;
		proj_cb.init(gfx, ConstantBufferType::Default, 0,&proj_struct);
		model_view_cb.init(gfx, ConstantBufferType::Dynamic, 1, &model_view_struct);
	}
	void render(entt::registry& reg, Graphics& gfx,Camera& camera)
	{
		proj_cb.bind(gfx);
		reg.group<Transform, Renderable>().each([&](entt::entity ent, Transform& trans, Renderable& rend) {
			if (trans.is_dirty)
			{
				trans.model = DirectX::XMMatrixIdentity();
				trans.model *= DirectX::XMMatrixScalingFromVector(trans.scale);
				trans.model *= DirectX::XMMatrixTranslationFromVector(trans.position);
				trans.model_inversed = DirectX::XMMatrixInverse(nullptr, trans.model);
				trans.is_dirty = false;
			}
			model_view_struct.view = camera.view_matrix;
			model_view_struct.model = trans.model;
			model_view_struct.model_inverse = trans.model_inversed;
			model_view_cb.update(gfx, &model_view_struct);
			model_view_cb.bind(gfx);
			rend.vbo.bind(gfx);
			rend.ibo.bind(gfx);
			gfx.context->DrawIndexed((UINT)rend.ibo.indices.size(), 0, 0 );
		});
	};
private:
	struct projection
	{
		dx::XMMATRIX projection;
	}proj_struct;
	struct model_view 
	{
		dx::XMMATRIX view;
		dx::XMMATRIX model;
		dx::XMMATRIX model_inverse;
	}model_view_struct;
	ConstantBuffer<projection> proj_cb;
	ConstantBuffer<model_view> model_view_cb;
};
