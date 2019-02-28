#pragma once

#include "Color_Buffer_Rgba8888.hpp"

#include <string>
#include <memory>

namespace przurro
{
	class Model;
	class Mesh;
	class Camera;
	class Color_Buffer_Rgba8888;

	using String = std::string;

	using Color = Color_Buffer_Rgba8888::Color;
	using Color_Buff = Color_Buffer_Rgba8888; //Color_Buffer
	using CColor_Buff = std::vector< Color_Buffer_Rgba8888::Color>; //Real color buffer

	//Shared Pointers
	using Model_sptr = std::shared_ptr<Model>;
	using Mesh_sptr = std::shared_ptr<Mesh>;
	using Camera_sptr = std::shared_ptr<Camera>;

	enum 
	{
		X,
		Y,
		Z,
		W,
	};

	struct Triangle_Index
	{
		size_t vA;
		size_t vB;
		size_t vC;
	};

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