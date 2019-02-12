#pragma once

#include "Declarations.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model
	{
	private:

		Model_Map meshes;

	private:
		String name;
		String error;

		Transform_Matrix3f	transform;

		Vector3f			position, rotation, constantRotation;
		float				scale;

		Vector3f color;

		Model * parent;		

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

	public:

		void translate(const Vector3f & translationV);
		void set_position(const Vector3f & positionV);

		void rotate(const Vector3f & rotationV);
		void set_rotation(const Vector3f & rotationV);
		void set_constant_rotation(const Vector3f & rotationV);

		void set_scale(const float & scaleF);

		void set_color(String & meshName, const Vector3f & colorV);
	};
}