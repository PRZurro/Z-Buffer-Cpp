#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "declarations/Containers.hpp"

#include "Rasterizer.hpp"

namespace przurro
{
	class Scene
	{
	private:

		Model_Map models;

		Camera_sptr activeCamera;

	private: 

		Rasterizer<Color_Buff>	rasterizer;
		Color_Buff	colorBuffer;


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
		bool load_model_property(XML_Node * attribute, Model & model);
	};

}