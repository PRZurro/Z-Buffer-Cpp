
#include "Scene_Object.hpp"

namespace przurro
{
	void Scene_Object::translate(const Vector3f & translationV)
	{
		position += translationV;
	}

	void Scene_Object::set_position(const Vector3f & positionV)
	{
		position = positionV;
	}

	void Scene_Object::rotate(const Vector3f & rotationV)
	{
		rotation += rotationV;
	}

	void Scene_Object::set_rotation(const Vector3f & rotationV)
	{
		rotation = rotationV;
	}

	void Scene_Object::set_constant_rotation(const Vector3f & rotationV)
	{
		constantRotation = rotationV;
	}

	void Scene_Object::set_scale(const float & scaleF)
	{
		scale = scaleF;
	}
	
	void Scene_Object::set_local_transform(const Transform_Matrix3f newTransform)
	{
		localTransform = newTransform;
	}
	void Scene_Object::set_transform(const Transform_Matrix3f newTransform)
	{
		globalTransform = newTransform;
	}
}
