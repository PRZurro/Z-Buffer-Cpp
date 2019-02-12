#pragma once

#include "Declarations.hpp"
#include "Vector.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Mesh
	{
	private:

		String name;
		
	private:

		Mesh_Attributes attributes;

	public:

		Mesh();
		Mesh(String inputName);

	public:

		void update(Projection_Matrix3f & projectionM);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);
	};
}