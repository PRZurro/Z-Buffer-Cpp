#include "Scene.hpp"
#include "Model.hpp"

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace rapidxml;
using namespace std;

namespace przurro
{
	Scene::Scene()
	{
	}

	void Scene::update(Projection_Matrix3f & projectionM)
	{
		for (Model_sptr & model : models)
		{
			model->update(projectionM);
		}
	}

	void Scene::draw(Rasterizer<Color_Buff>& rasterizer)
	{
	}


	bool Scene::load_scene(String xmlFilePath)
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
					XML_Attr * attribute = model->first_attribute("path");

					if (!attribute)
						return false;

					if(attribute->name() != "path")
						return false;

					models.push_back(Model_sptr(new Model(attribute->value())));

					XML_Node * property = model->first_node();

					for (property; property; property = property->next_sibling())
					{
						if (!property || property->type() != node_element)
							return false;

						load_model_attribute(property, *models.back());
					}
				}
			}
			else return false;	
		}

		return true;
	}
	bool Scene::load_model_attribute(XML_Node * attribute, Model & model)
	{
		String name = attribute->name();
		String value = attribute->value();

		if (name == "position" || name == "rotation" || name == "color" || name == "constant_rotation")
		{
			vector<String> stringChunks  = String_Utilities::string_splitter(value, ',');
			Vector3f values ({stof(stringChunks[0]),stof(stringChunks[1]),stof(stringChunks[2])});

			if (stringChunks.size() == 3)
			{
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
				else if (name == "color")
				{
					model.set_color(values);
				}


				return true;
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