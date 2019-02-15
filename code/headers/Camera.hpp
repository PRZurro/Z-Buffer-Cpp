#pragma once

#include "declarations/Math.hpp"

namespace przurro
{
	class Camera
	{
	private:

		Transform_Matrix3f transform;
		Projection_Matrix3f projectionMatrix;

	public:

		Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t width, size_t height);

	public:

		Projection_Matrix3f & get_projection_matrix()
		{
			return projectionMatrix;
		}
	};
}

