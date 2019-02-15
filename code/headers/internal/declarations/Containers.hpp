#pragma once

#include <unordered_map>
#include <list>

namespace przurro
{
	using Mesh_List = std::list<Mesh_sptr>;
	using Mesh_Map = std::unordered_map<String, Mesh_sptr>;

	using Model_List = std::list<Model_sptr>;
	using Model_Map = std::unordered_map<String, Model_sptr>;

	using i_Buffer = std::vector<int>;
	using f_Buffer = std::vector<float>;

	using Point4f_Buffer = std::vector<Point4f>;
	using Point4i_Buffer = std::vector <Point4i>;
	using Vector4f_Buffer = std::vector<Vector4f>;
}