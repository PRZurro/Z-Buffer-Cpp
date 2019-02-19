
#include "Scene_Object.hpp"

namespace przurro
{
	Scene_Object::Scene_Object(const String & inputName) 
		: name(inputName),
		position({ 0.f, 0.f, 0.f}),
		rotation({ 0.f, 0.f, 0.f }),
		constantRotation({ 0.f,0.f,0.f }),
		scale(1)
	{

	}
	void Scene_Object::translate(const Vector3f & translationV)
	{
		position += translationV;

		update_transform();
	}

	void Scene_Object::set_position(const Vector3f & positionV)
	{
		position = positionV;

		update_transform();
	}

	void Scene_Object::rotate(const Vector3f & rotationV)
	{
		rotation += rotationV;

		update_transform();
	}

	void Scene_Object::set_rotation(const Vector3f & rotationV)
	{
		rotation = rotationV;

		update_transform();
	}

	void Scene_Object::set_constant_rotation(const Vector3f & rotationV)
	{
		constantRotation = rotationV;
	}

	void Scene_Object::set_scale(const float & scaleF)
	{
		scale = scaleF;

		update_transform();
	}
	
	void Scene_Object::set_local_transform(const Transform_Matrix3f newTransform)
	{
		localTransform = newTransform;
	}
	void Scene_Object::set_transform(const Transform_Matrix3f newTransform)
	{
		globalTransform = newTransform;
	}

	void Scene_Object::set_parent(Transform_Matrix3f & parent)
	{
		transformParent = &parent;
	}

	void Scene_Object::update_transform()
	{
		Scale_Matrix3f scaleMatrix(scale);
		Translation_Matrix3f positionMatrix(position);
		Rotation_Matrix3f rotationX, rotationY, rotationZ;

		rotationX.set< Rotation_Matrix3f::AROUND_THE_X_AXIS >(rotation[X]);
		rotationY.set< Rotation_Matrix3f::AROUND_THE_Y_AXIS >(rotation[Y]);
		rotationZ.set< Rotation_Matrix3f::AROUND_THE_Z_AXIS >(rotation[Z]);

		globalTransform = localTransform = positionMatrix * rotationX * rotationY * rotationZ * scaleMatrix;

		if(transformParent)
			globalTransform = globalTransform * (*transformParent);

	}

}