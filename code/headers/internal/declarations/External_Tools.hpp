#pragma once

#include "rapidxml.hpp"
#include "tiny_obj_loader.h"

namespace przurro
{
	using XML_Node = rapidxml::xml_node<>;
	using XML_Attr = rapidxml::xml_attribute<>;

	using Shape_t = tinyobj::shape_t;
	using Attrib_t = tinyobj::attrib_t;
	using Index_t = tinyobj::index_t;
	using Material_t = tinyobj::material_t;
}