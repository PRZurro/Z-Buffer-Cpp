#include "Camera.hpp"

namespace przurro
{
	Camera::Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t width, size_t height)
		:projectionMatrix(nearPlaneD, farPlaneD, fovDegrees, width* 1/height)
	{
	}
}