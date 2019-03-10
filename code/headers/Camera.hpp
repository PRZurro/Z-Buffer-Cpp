/**
 * @file Camera.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-14
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Math.hpp"
#include "declarations/Containers.hpp"

#include "Scene_Object.hpp"

namespace przurro
{
	/**
	 * @brief Class in charge of store the attributes belonging to the characteristics of a camera, and manage it's changes
	 * 
	 */
	class Camera : public Scene_Object
	{
	private:

		Projection_Matrix3f projectionMatrix; 
		 
		Vector3f * viewTarget;
		Vector3f defaultViewDirection; 

		size_t width;
		size_t height;

		Vector4f_Buffer fPlanes; //Planes that compose the frustum

	public:

		Camera(float nearPlaneD, float farPlaneD, float fovDegrees, size_t inputWidth, size_t inputHeight, Vector3f * target = nullptr);

	public:

		/**
		 * @brief Changes the direction where this camera is looking at and returns the proper camera coordinates
		 * 
		 * @param at: vector to look at
		 * @return Matrix44f, camera coordinates transformation 
		 */
		Matrix44f look_at(const Vector3f& at);

		/**
		 * @brief Set a direction to look at and returns the camera coordinates returned
		 * 
		 * If there is not any target, it takes the default view direction 
		 * 
		 * @return Matrix44f camera coordinates transformation
		 */
		Matrix44f look_at()
		{
			if (!viewTarget)
			{
				return look_at(defaultViewDirection);
			}

			return look_at(*viewTarget);
		}

	public:

		/**
		 * @brief Set the target object
		 * 
		 * @param target 
		 */
		void set_target(Vector3f * target)
		{
			viewTarget = target;
		}

	public:

		/**
		 * @brief Get the projection matrix object
		 * 
		 * @return const Projection_Matrix3f& 
		 */
		const Projection_Matrix3f & get_projection_matrix() const
		{
			return projectionMatrix;
		}

		/**
		 * @brief Get the width object
		 * 
		 * @return size_t 
		 */
		size_t get_width()
		{
			return width;
		}

		/**
		 * @brief Get the height object
		 * 
		 * @return size_t 
		 */
		size_t get_height()
		{
			return height;
		}

		/**
		 * @brief Get the calculated frustum planes
		 * 
		 * @param normalized 
		 * @return Vector4f_Buffer& 
		 */
		Vector4f_Buffer & get_frustum_planes(bool normalized = false)
		{
			update_frustum_planes(normalized);
			return fPlanes;
		}

	private:

		/**
		 * @brief Update the frustum planes with the current transformation and the projection matrix
		 * 
		 * @param normalize 
		 */
		void update_frustum_planes(bool normalize = false);
	};
}