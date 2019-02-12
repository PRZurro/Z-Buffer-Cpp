#pragma once

#include <Projection.hpp>
#include <Point.hpp>
#include <Translation.hpp>
#include <Rotation.hpp>
#include <Scaling.hpp>
#include <Vector.hpp>
#include <Transformation.hpp>
#include "Color_Buffer_Rgba8888.hpp"

#include <rapidxml.hpp>

#include <vector>
#include <memory>
#include <list>

namespace przurro
{
	class Model;
	class Mesh;

	using String = std::string;

	using Point2f = toolkit::Point2f;
	using Point4f = toolkit::Point4f;
	using Point4i = toolkit::Point4i;

	using Vector3f = toolkit::Vector3f;
	using Vector4f = toolkit::Vector4f;
	using Vector3i = toolkit::Vector3i;
	using Vector4i = toolkit::Vector4i;

	using Matrix44f = toolkit::Matrix44f;
	using Matrix41f = toolkit::Matrix41f;

	using Color = Color_Buffer_Rgba8888::Color;

	using Model_sptr = std::shared_ptr<Model>;
	using Model_List = std::list<std::shared_ptr<Model>>;
	using Mesh_stpr = std::shared_ptr<Mesh>;
	using Mesh_List = std::list<std::shared_ptr<Mesh>>;

	using Point4f_Buffer = std::vector<toolkit::Point4f>;
	using Point4i_Buffer = std::vector <toolkit::Point4i>;
	using Vector3f_Buffer = std::vector<toolkit::Vector3f>;
	using Int_Buffer = std::vector<int>;
	using Color_Buff = Color_Buffer_Rgba8888; //Color_Buffer

	using Translation_Matrix3f = toolkit::Translation3f;
	using Rotation_Matrix3f = toolkit::Rotation3f;
	using Scale_Matrix3f = toolkit::Scaling3f;
	using Projection_Matrix3f = toolkit::Projection3f;

	using Transform_Matrix3f = toolkit::Transformation3f;

	using XML_Node = rapidxml::xml_node<>;
	using XML_Attr = rapidxml::xml_attribute<>;

	enum 
	{
		X,
		Y,
		Z
	};

	typedef struct Mesh_Attributes
	{
		Point4f_Buffer		originalVertices;   // "ov"
		Vector3f_Buffer		originalNormals;
		Color_Buff			originalColors;
		
		Int_Buffer			originalIndices; // 'oi'

		Point4f_Buffer		transformedVertices;
		Vector3f_Buffer		transformedNormals;
		Point4i_Buffer		displayVertices;

		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'
	};
}