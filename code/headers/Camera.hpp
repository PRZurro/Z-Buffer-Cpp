#pragma once

#include "declarations/Math.hpp"

#include "Scene_Object.hpp"

namespace przurro
{
	class Camera : public Scene_Object
	{
	private:

		Projection_Matrix3f projectionMatrix;

		size_t width;
		size_t height;

	public:

		Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t inputWidth, size_t inputHeight);

	public:

		Projection_Matrix3f & get_projection_matrix()
		{
			return projectionMatrix;
		}

		size_t get_width()
		{
			return width;
		}
		size_t get_height()
		{
			return height;
		}
	};
}