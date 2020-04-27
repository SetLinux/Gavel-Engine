#include "camera.h"

Camera::Camera(camera_desc& desc)
{
	if (desc.camera_type == CameraType::Orthographic)
	{
		projection_matrix = dx::XMMatrixPerspectiveFovLH(desc.perspective.fov, desc.perspective.aspect_ratio, desc.near_z, desc.far_z);
	}
	else
	{
		projection_matrix = dx::XMMatrixOrthographicLH(desc.orthographic.width, desc.orthographic.height, desc.near_z, desc.far_z);
	}
}

Camera::~Camera()
{
}

void Camera::set_position(dx::XMVECTOR& position)
{
	this->position = position;
	refresh_view_matrix();
}
void Camera::set_orientation(float yaw, float pitch, float roll)
{
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
	refresh_view_matrix();
}

void Camera::refresh_view_matrix()
{	dx::XMVECTOR direction{ DirectX::XMScalarCos(DirectX::XMConvertToRadians(yaw)) * DirectX::XMScalarCos(DirectX::XMConvertToRadians(pitch)),DirectX::XMScalarSin(DirectX::XMConvertToRadians(pitch)) ,DirectX::XMScalarSin(DirectX::XMConvertToRadians(yaw)) * DirectX::XMScalarCos(DirectX::XMConvertToRadians(pitch))  , 0.0 };
	view_matrix = dx::XMMatrixIdentity();
	view_matrix = dx::XMMatrixLookAtLH(position, dx::XMVectorAdd(position , direction), { 0.0f,1.0f,0.0f });
}

