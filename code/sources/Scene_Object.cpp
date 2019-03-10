
#include "Scene_Object.hpp"
#include "gtx/matrix_decompose.hpp"

namespace przurro
{
	Scene_Object::Scene_Object(const String & inputName)
		: name(inputName),
		position(Vector3f({0.f, 0.f, 0.f})),
		rotation(position),
		constantRotation(position),
		scale(1.f),
		localTransform(Matrix44f::identity),
		globalTransform(localTransform),
		transformParent(nullptr)
	{}

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

	void Scene_Object::constant_rotation()
	{
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
		rotation += constantRotation;
		Scale_Matrix3f scaleMatrix(scale);
		Translation_Matrix3f positionMatrix(position);
		Rotation_Matrix3f rotationX, rotationY, rotationZ;

		rotationX.set< Rotation_Matrix3f::AROUND_THE_X_AXIS >(rotation[X]);
		rotationY.set< Rotation_Matrix3f::AROUND_THE_Y_AXIS >(rotation[Y]);
		rotationZ.set< Rotation_Matrix3f::AROUND_THE_Z_AXIS >(rotation[Z]);

		//Creation of the unified transformation matrix :

		globalTransform = localTransform = positionMatrix * rotationX * rotationY * rotationZ * scaleMatrix;

		if (transformParent)
		{
			Transform_Matrix3f parent = *transformParent;
			globalTransform = parent * globalTransform;
		}

		update_global_attributes(); // Can`t extract the position properly for the moment
	}
	void Scene_Object::update_global_attributes()
	{
		glm::mat4 transformation; // your transformation matrix.
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				Matrix44f tempMatrix(globalTransform);
				transformation[i][j] = tempMatrix[i][j];
			}
		}

		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		for (size_t i = 0; i < 3; i++)
		{
			gPosition[i] = translation[i];
		}

	}

}
