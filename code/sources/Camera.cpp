#include "Camera.hpp"

namespace przurro
{
	Camera::Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t inputWidth, size_t inputHeight)
		: width(inputWidth), 
		height(inputHeight), 
		projectionMatrix(nearPlaneD, farPlaneD, fovDegrees, width* (1/height))
	{}
}