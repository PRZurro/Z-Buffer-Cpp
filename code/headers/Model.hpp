#pragma once

#include "declarations/Internal.hpp"

#include "Rasterizer.hpp"

namespace przurro
{
	class Model
	{


	private:

		String name;
		String error;

	private:

		Mesh_Map meshes;

		Point4f_Buffer originalVertices;
		Vector4f_Buffer	ovNormals; // original vertices normals, 4th component must be set to '0.f'

		Color defaultColor;

	private:

		Transform_Matrix3f	localTransform;
		Transform_Matrix3f * transformParent; // Could be the camera's transform or another model's transform (that will ultimately inherit from the camera's transform)

		Vector3f			position, rotation, constantRotation;
		float				scale;
		
	public:

		Model(const String & assetFolderPath, const String & assetName);

	public:

		void update(Camera * activeCamera);

		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		void translate(const Vector3f & translationV);
		void set_position(const Vector3f & positionV);

		void rotate(const Vector3f & rotationV);
		void set_rotation(const Vector3f & rotationV);
		void set_constant_rotation(const Vector3f & rotationV);

		void set_scale(const float & scaleF);

		bool set_mesh_color(String & meshName, const Vector3f & colorV);

		void set_local_transform(const Transform_Matrix3f newTransform);
		void set_transform(const Transform_Matrix3f newTransform);

	public:

		bool is_ok()
		{
			return error.empty();
		}

		const String get_error() const
		{
			return error;
		}

		const Color get_default_color()const
		{
			return defaultColor;
		}
	};
}