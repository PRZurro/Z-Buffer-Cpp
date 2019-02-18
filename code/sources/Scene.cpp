#include "Scene.hpp"
#include "Model.hpp"
#include "Camera.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace rapidxml;
using namespace std;

namespace przurro
{
	Scene::Scene(String & inputAssetsFolderPath, size_t inputWidth, size_t inputHeight, float nearPlaneD, float farPlaneD, float fovDegrees)
		: assetsFolderPath(inputAssetsFolderPath),
		colorBuffer(inputWidth,inputHeight), 
		rasterizer(colorBuffer), 
		activeCamera(new Camera(nearPlaneD ,farPlaneD,fovDegrees,inputWidth,inputHeight))
	{
	}

	void Scene::update()
	{
		for(auto & model : models)
		{
			model.second->update(activeCamera.get());
		}
	}

	void Scene::draw()
	{
		rasterizer.clear();

		for (auto & model : models)
		{
			model.second->draw(rasterizer);
		}

		// Se copia el frameb�ffer oculto en el frameb�ffer de la ventana:

		rasterizer.get_color_buffer().gl_draw_pixels(0, 0);
	}

	bool Scene::load_scene(String & xmlFilePath)
	{
		bool loaded = false;

		rapidxml::xml_document<> doc;
		XML_Node * rootNode;

		// Read the xml file into a vector
		ifstream theFile(xmlFilePath);
		vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');

		// parse the buffer using the xml file parsing library into doc 
		doc.parse<0>(&buffer[0]);
		//find our root node
		rootNode = doc.first_node("scene");

		if (!rootNode || rootNode->first_node("models")->type() != node_element)
		{
			return false;
		}

		if (rootNode->first_node("models")->name() != "models")
			return false;

		return load_models(rootNode->first_node("models"));
	}

	bool Scene::load_models(XML_Node * modelNodeRoot)
	{
		XML_Node * model = modelNodeRoot->first_node("model");

		if (!model || model->type() != node_element)
		{
			return false;
		}

		if (model->first_node("models")->name() != "model")
			return false;

		for (model; model; model = model->next_sibling())
		{
			if (model->type() == node_element)
			{
				// Iterate over each entity node
				String name = "";
				if (String(model->name()) == "model")
				{
					XML_Attr * attribPath = model->first_attribute("path"), * attribName = model->first_attribute("name");;
					
					if (!attribPath || !attribName)
						return false;

					if(attribPath->name() != "path" || attribName->name() != "name")
						return false;

					models[name] = Model_sptr(new Model("folderPath", attribName->value()));

					XML_Node * modelProperty = model->first_node();

					for (modelProperty; modelProperty; modelProperty = modelProperty->next_sibling())
					{
						if (!modelProperty || modelProperty->type() != node_element)
							return false;

						load_model_property(modelProperty, *models[attribName->value()]);
					}
				}
			}
			else return false;	
		}

		return true;
	}
	bool Scene::load_model_property(XML_Node * node, Model & model)
	{
		String name = node->name();
		String value = node->value();

		if (name == "position" || name == "rotation" || name == "color" || name == "constant_rotation")
		{
			vector<String> stringChunks  = String_Utilities::string_splitter(value, ',');
			
			if (stringChunks.size() == 3)
			{
				Vector3f values({ stof(stringChunks[0]),stof(stringChunks[1]),stof(stringChunks[2]) });

				if (name == "position")
				{
					model.set_position(values);
				}
				else if(name == "rotation")
				{
					model.set_rotation(values);
				}
				else if (name == "constant_rotation")
				{
					model.set_constant_rotation(values);
				}
			}
			else if (stringChunks.size() == 4)
			{
				Vector4f values({ stof(stringChunks[X]),stof(stringChunks[Y]),stof(stringChunks[Z]), stof(stringChunks[W]) });

				if (name == "default_color")
					model.set_default_color(values);

				if (name == "mesh_color")
				{
					XML_Attr * attribName = node->first_attribute("mesh_name");

					if (!attribName)
						return false;
					if (attribName->name() != "mesh_name")
						return false;

					String meshName = attribName->value();

					model.set_mesh_color(meshName, values);
				}
			}
		}
		else if (name == "scale") 
		{
			model.set_scale(stof(value));
			return true;
		}

		return false;
	}
}