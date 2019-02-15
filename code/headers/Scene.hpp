#pragma once

#include "Declarations.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model;
	
	class Scene
	{
	private:

		Model_Map models;
		
		Camera_sptr activeCamera;

	public:

		Scene();

		~Scene()
		{
			models.clear();
		}

	public:

		void update(Camera * activeCamera);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool load_scene(String xmlFilePath);
		bool load_models(XML_Node * modelNodeRoot);
		bool load_model_attribute(XML_Node * attribute, Model & model);
	};

}