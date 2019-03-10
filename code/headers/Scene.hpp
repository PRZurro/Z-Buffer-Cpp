/**
 * @file Scene.hpp
 * @author Pablo Rodríguez Zurro (przuro@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-02-05
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "declarations/Containers.hpp"

#include "Light.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	/**
	 * @brief Core of the application. Parse and load xml documents, update it's created models and meshes and manage the rasterizer to render it's members
	 * 
	 */
	class Scene
	{
	private:

		Model_Map models; 
		Camera_sptr activeCamera;
		Light infiniteLight; //Directional light set in the infinite

	private: 

		Color_Buff	colorBuffer; //Color buffer needed by the rasterizer 
		Rasterizer<Color_Buff>	rasterizer; //Object in charge of render the scene (vertices, pixels, colors...)

	private:

		String assetsFolderPath;

	public:

		/**
		 * @brief Construct a new Scene object
		 * 
		 * @param inputAssetsFolderPath 
		 * @param inputWidth 
		 * @param inputHeight 
		 * @param nearPlaneD 
		 * @param farPlaneD 
		 * @param fovDegrees 
		 */
		Scene(String & inputAssetsFolderPath, size_t inputWidth = 800, size_t inputHeight = 600, float nearPlaneD = .4f, float farPlaneD = 1000.f, float fovDegrees = 90.f);

		/**
		 * @brief Destroy the Scene object (clearing the models buffer)
		 * 
		 */
		~Scene()
		{
			models.clear();
		}

	public:

		/**
		 * @brief update the scene
		 * 
		 */
		void update();

		/**
		 * @brief draw the scene
		 * 
		 */
		void draw();

	public:

		/**
		 * @brief method in charge of parse and load all objects in the xml located in the path received
		 * 
		 * @param xmlFilePath 
		 * @return true 
		 * @return false 
		 */
		bool load_scene(String & xmlFilePath);

		/**
		 * @brief load all models (with their properties)
		 * 
		 * @param modelNodeRoot 
		 * @return true 
		 * @return false 
		 */
		bool load_models(XML_Node * modelNodeRoot);

		/**
		 * @brief load all the cameras (with their properties)
		 * 
		 * @param cameraNode 
		 * @return true 
		 * @return false 
		 */
		bool load_cameras(XML_Node * cameraNode);

		/**
		 * @brief Common method to load properties of models and cameras (sometimes is needed a pointer to a model and a camera. E.g: setting the parent of the camera)
		 * 
		 * @param attribute 
		 * @param model 
		 * @param camera 
		 * @return true 
		 * @return false 
		 */
		bool load_property(XML_Node * attribute, Model * model = nullptr, Camera * camera = nullptr);

	public:

		/**
		 * @brief Check if there it is a model with the name received
		 * 
		 * @param name 
		 * @return true 
		 * @return false 
		 */
		bool model_exists(const String & name);
		
	};
}