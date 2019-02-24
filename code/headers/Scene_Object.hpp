#pragma once

#include "declarations/Math.hpp"
#include "declarations/Internal.hpp"

namespace przurro
{
	class Scene_Object
	{
	protected:

		String name;
		//Scene scene;

	protected:

		Transform_Matrix3f	localTransform;
		Transform_Matrix3f	globalTransform;

		Transform_Matrix3f * transformParent; // Could be the camera's transform or another model's transform (that will ultimately inherit from the camera's transform)

		Vector3f			position, rotation, constantRotation;
		float				scale;

	public:

		Scene_Object(const String & inputName);

	public:

		void translate(const Vector3f & translationV);
		void set_position(const Vector3f & positionV);

		void rotate(const Vector3f & rotationV);
		void set_rotation(const Vector3f & rotationV);
		void set_constant_rotation(const Vector3f & rotationV);

		void set_scale(const float & scaleF);

		void set_local_transform(const Transform_Matrix3f newTransform);
		void set_transform(const Transform_Matrix3f newTransform);

		void set_parent(Transform_Matrix3f & parent);

	public:

		Transform_Matrix3f * get_parent() const
		{
			return transformParent;
		}

		Transform_Matrix3f & get_transform()
		{
			return globalTransform;
		}

		Vector3f & get_reference_to_position()
		{
			return position;
		}

		Transform_Matrix3f get_inverse_transform() const
		{
			Transform_Matrix3f inverseMatrix = globalTransform;
		
			return inverseMatrix;
		}

	private:

		void update_transform();
	};
}