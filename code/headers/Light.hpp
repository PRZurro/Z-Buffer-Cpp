/**
 * @file Light.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-15
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Math.hpp"

namespace przurro
{
	/**
	 * @brief Class in charge of store attributes belonging to the characteristics of a light
	 * 
	 */
	class Light
	{
	private:

		float intensity;
		Vector4f direction;

	public:

		/**
		 * @brief Construct a new Light object by default
		 * 
		 */
		Light(): intensity(4.f), direction({1.f, 1.f, 0.f, 0.f}){}

		/**
		 * @brief Construct a new Light object
		 * 
		 * @param inputIntensity 
		 * @param inputDirection 
		 */
		Light(float inputIntensity, Vector4f & inputDirection) : intensity(inputIntensity), direction(inputDirection){}

	public:

		/**
		 * @brief Set the intensity object
		 * 
		 * @param inputIntensity 
		 */
		void set_intensity(float inputIntensity)
		{
			intensity = inputIntensity;
		}

		/**
		 * @brief Set the direction object
		 * 
		 * @param inputDirection 
		 */
		void set_direction(Vector4f & inputDirection)
		{
			direction = inputDirection;
			direction[3] = 0.f;
		}

	public:

		/**
		 * @brief Get the intensity object
		 * 
		 * @return float 
		 */
		float get_intensity()const
		{
			return intensity;
		}

		/**
		 * @brief Get the direction object
		 * 
		 * @return Vector4f 
		 */
		Vector4f get_direction()const
		{
			return direction;
		}
	};
}