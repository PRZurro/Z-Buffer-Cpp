/**
 * @file Scene_Object.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-10
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Math.hpp"
#include "declarations/Internal.hpp"

namespace przurro
{	

	/**
	 * @brief Class similar to a component in charge of store manage the transformations of an object of a scene. 
	 * 
	 */
	class Scene_Object
	{
	protected:

		String name;
		//Scene scene;

	protected:

		Transform_Matrix3f	localTransform; 							//Model coordinates
		Transform_Matrix3f	globalTransform; 							//Scene coordinates 

		Transform_Matrix3f * transformParent; 							// Could be the camera's transform or another model's transform (that will ultimately inherit from the camera's transform)

		Vector3f			position, rotation, constantRotation;
		Vector3f			gPosition, gScale; 							// global transformation attributes
		float				scale;

	public:

		/**
		 * @brief Construct a new Scene_Object object
		 * 
		 * @param inputName 
		 */
		Scene_Object(const String & inputName);

	public:

		/**
		 * @brief translate this scene object.
		 * 
		 * @param translationV 
		 */
		void translate(const Vector3f & translationV);

		/**
		 * @brief Set the position object
		 * 
		 * @param positionV 
		 */
		void set_position(const Vector3f & positionV);

		/**
		 * @brief rotate this scene object.
		 * 
		 * @param rotationV 
		 */
		void rotate(const Vector3f & rotationV);

		/**
		 * @brief Set the rotation object
		 * 
		 * @param rotationV 
		 */
		void set_rotation(const Vector3f & rotationV);

		/**
		 * @brief Set the constant rotation object
		 * 
		 * @param rotationV 
		 */
		void set_constant_rotation(const Vector3f & rotationV);

		/**
		 * @brief Set the scale object
		 * 
		 * @param scaleF 
		 */
		void set_scale(const float & scaleF);

		/**
		 * @brief Set the local transform object
		 * 
		 * @param newTransform 
		 */
		void set_local_transform(const Transform_Matrix3f newTransform);

		/**
		 * @brief Set the transform object
		 * 
		 * @param newTransform 
		 */
		void set_transform(const Transform_Matrix3f newTransform);

		/**
		 * @brief Set the parent object
		 * 
		 * @param parent 
		 */
		void set_parent(Transform_Matrix3f & parent);

	public:

		/**
		 * @brief apply the constant rotation to the rotation. (It only calls to update the transform, because is the real the real responsible for this task)
		 * 
		 */
		void constant_rotation();

	public: 

		/**
		 * @brief Get the name object
		 * 
		 * @return const String 
		 */
		const String get_name() const
		{
			return name;
		}

		/**
		 * @brief Get the parent object
		 * 
		 * @return Transform_Matrix3f* 
		 */
		Transform_Matrix3f * get_parent() const
		{
			return transformParent;
		}

		/**
		 * @brief Get a reference to this global transform object
		 * 
		 * @return Transform_Matrix3f& 
		 */
		Transform_Matrix3f & get_transform()
		{
			return globalTransform;
		}

		/**
		 * @brief Get the reference to position object
		 * 
		 * @return Vector3f& 
		 */
		Vector3f & get_reference_to_position()
		{
			//return gPosition; //Due to not to being able to extract the position for the moment, it is necessary to return the initial position.
			return position;
		}

	public:

		/**
		 * @brief Update the local and global transform by the current local attributes and parent transformation. Can't extract for the moment the global values. 
		 * 
		 */
		void update_transform();

	private:

		/**
		 * @brief Used to extrac current world attributes. Not working
		 * 
		 */
		void update_global_attributes();
	};
}