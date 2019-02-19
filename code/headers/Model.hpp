#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "Mesh.hpp"

#include "Scene_Object.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model : public Scene_Object
	{
	private:

		Mesh_Map meshes;

		Point4f_Buffer ovPositions;
		Vector4f_Buffer	ovNormals; // original vertices normals, 4th component must be set to '0.f'

		Color defaultColor;
		
	private:

		String error;

	public:

		Model(const String & assetFolderPath, const String & assetName);

		~Model()
		{
			ovPositions.clear();
			ovNormals.clear();
		}

	public:

		void update(Camera * activeCamera);

		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool set_mesh_color(String & meshName, const Vector4f & colorV);
		void set_default_color(const Vector4f & colorRGB);

	public:

		bool is_ok()
		{
			return error.empty();
		}

		const String get_error() const
		{
			return error;
		}

		const Color get_default_color()const
		{
			return defaultColor;
		}
	};
}