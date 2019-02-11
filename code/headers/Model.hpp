#pragma once

#include <vector>
#include <string>

#include "Color_Buffer_Rgba8888.hpp"
#include "Rasterizer.hpp"

#include "Projection.hpp"
#include "Transformation.hpp"
#include "Rotation.hpp"
#include "Scaling.hpp"

namespace przurro
{
	class Model
	{

	private:

		std::string error;

	public:

		Model(const std::string & modelFilePath);

	public:

		bool is_ok()
		{
			return error.empty();
		}

		const std::string get_error() const
		{
			return error;
		}
	};
}

