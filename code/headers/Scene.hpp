#pragma once

#include "Declarations.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model;
	
	class Scene
	{
	private:

		Model_List models;

	public:

		Scene();

	public:

		void update(Projection_Matrix3f & projectionM);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool load_scene(String xmlFilePath);
		bool load_models(XML_Node * modelNodeRoot);
		bool load_model_attribute(XML_Node * attribute, Model & model);
	};

}