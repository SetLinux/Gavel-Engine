#pragma once
#include <DirectXMath.h>
namespace dx = DirectX;
enum class CameraType : unsigned int
{
	Orthographic = 0,
	Projeciton = 1
};
struct orthographic_desc
{
	float width{ 0.0f }, height{ 0.0f };
};
struct perspective_desc
{
	float fov{ 0.0f }, aspect_ratio{ 0.0f };
};
struct camera_desc
{
	CameraType camera_type;
	orthographic_desc orthographic;
	perspective_desc perspective;
	float near_z{ 0.0f };
	float far_z{ 0.0f };
};
class Camera
{
public:
	Camera(camera_desc& desc);
	~Camera();
	void set_position(dx::XMVECTOR& position);
	void set_orientation(float yaw, float pitch, float roll);
	dx::XMMATRIX view_matrix;
	dx::XMMATRIX projection_matrix;
private:
	dx::XMVECTOR position;
	float yaw, pitch, roll;
private:
	void refresh_view_matrix();
};
