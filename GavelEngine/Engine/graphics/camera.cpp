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
