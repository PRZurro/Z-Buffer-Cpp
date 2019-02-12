#include "Scene.hpp"
#include "Model.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>

przurro::Scene::Scene()
{
}

void przurro::Scene::update(Projection_Matrix3f & projectionM)
{
	for (Model_sptr & model : models)
	{
		model->update(projectionM);
	}
}

void przurro::Scene::draw(Rasterizer<Color_Buff>& rasterizer)
{
}

using namespace std;
bool przurro::Scene::load_scene(String xmlFilePath)
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

	if (!rootNode)
	{
		return false;
	}
	
	load_models(rootNode->first_node("models"));

	return true;
}

bool przurro::Scene::load_models(XML_Node * modelNodeRoot)
{


	return true;
}

