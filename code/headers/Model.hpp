#pragma once

#include "Declarations.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model
	{
	private:

		Mesh_List meshes;

	private:

		String error;

	public:

		Model(const String & modelFilePath);

	public:

		void update(Projection_Matrix3f & projectionM);

		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_ok()
		{
			return error.empty();
		}

		const String get_error() const
		{
			return error;
		}
	};
}