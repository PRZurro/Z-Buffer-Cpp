#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "declarations/Containers.hpp"

#include "Light.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Scene
	{
	private:

		Model_Map models;

		Camera_sptr activeCamera;

		Light infiniteLight;

	private: 

		Color_Buff	colorBuffer;

		Rasterizer<Color_Buff>	rasterizer;

	private:

		String assetsFolderPath;

	public:

		Scene(String & inputAssetsFolderPath, size_t inputWidth = 800, size_t inputHeight = 600, float nearPlaneD = .4f, float farPlaneD = 1000.f, float fovDegrees = 90.f);

		~Scene()
		{
			models.clear();
		}

	public:

		void update();
		void draw();

	public:

		bool load_scene(String & xmlFilePath);
		bool load_models(XML_Node * modelNodeRoot);
		bool load_cameras(XML_Node * cameraNode);
		bool load_property(XML_Node * attribute, Model * model = nullptr, Camera * camera = nullptr);

	public:

		bool is_existing_model(const String & name);
		
	};
}