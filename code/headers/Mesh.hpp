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

		Transform_Matrix3f	transform;

		Vector3f			position, rotation; 
		float				scale;
		
	private:

		Mesh_Attributes attributes;

	public:

		Mesh();
		Mesh(String inputName = "undefined");

	public:

		void update(Projection_Matrix3f * projectionM);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);

	public:

		void translate(const Vector3f & translationV);
		void set_position(const Vector3f & positionV);

		void rotate(const Vector3f & rotationV);
		void set_rotation(const Vector3f & rotationV);

		void set_scale(const float & scaleF);
	};
}