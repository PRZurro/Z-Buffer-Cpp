#pragma once

#include "declarations/Math.hpp"

namespace przurro
{
	class Light
	{
	private:

		float intensity;
		Vector4f direction;

	public:

		Light(): intensity(4.f), direction({1.f, 1.f, 0.f, 0.f}){}
		Light(float inputIntensity, Vector4f & inputDirection) : intensity(inputIntensity), direction(inputDirection){}

	public:

		void set_intensity(float inputIntensity)
		{
			intensity = inputIntensity;
		}

		void set_direction(Vector4f & inputDirection)
		{
			direction = inputDirection;
			direction[3] = 0.f;
		}

	public:

		float get_intensity()const
		{
			return intensity;
		}

		Vector4f get_direction()const
		{
			return direction;
		}
	};
}