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
#include <unordered_map>
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
	using Model_Map = std::unordered_map<String, Model_sptr>;

	using Mesh_sptr = std::shared_ptr<Mesh>;
	using Mesh_List = std::list<std::shared_ptr<Mesh>>;
	using Mesh_Map = std::unordered_map<String, Model_sptr>;

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

	typedef struct 
	{
		Point4f_Buffer		originalVertices;
		Vector3f_Buffer		originalNormals;
		Color_Buff			originalColors;
		
		Int_Buffer			originalIndices;

		Point4f_Buffer		transformedVertices;
		Vector3f_Buffer		transformedNormals;
		Point4i_Buffer		displayVertices;

		Color				color; // Main color of the vertices

		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'
	}Mesh_Attributes;

	class String_Utilities
	{
	public:

		static std::vector<String> string_splitter(const String & _toSplit, char _delimitier)
		{
			std::vector<String> tempVector;

			String tempstring = _toSplit;

			String::size_type i = 0;
			String::size_type j = tempstring.find(_delimitier);

			if (j == String::npos)
			{
				tempVector.push_back(tempstring.substr(i, tempstring.length()));
				return tempVector;
			}

			while (j != String::npos)
			{
				tempVector.push_back(tempstring.substr(i, j - i));
				i = ++j;
				j = tempstring.find(_delimitier, j);

				if (j == String::npos)
					tempVector.push_back(tempstring.substr(i, tempstring.length()));
			}
			return tempVector;
		}
	};
}