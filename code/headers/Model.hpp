#pragma once

#include "declarations/Internal.hpp"
#include "declarations/External_Tools.hpp"
#include "Mesh.hpp"
#include "Light.hpp"

#include "Scene_Object.hpp"
#include "Rasterizer.hpp"

namespace przurro
{
	class Model : public Scene_Object
	{
	private:

		Mesh_Map meshes;

		Point4f_Buffer		ovPositions;
		Vector4f_Buffer		ovNormals; // original vertices normals, 4th component must be set to '0.f'
		i_Buffer			meshesVerticesN;

		i_Buffer			trianglesIntensities;
		Point4f_Buffer		tvPositions;
		Vector4f_Buffer		tvNormals;


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

		void update(Camera * activeCamera, Light * inputLight);

		void draw(Rasterizer<Color_Buff> & rasterizer);
		
	public:

		bool set_mesh_color(String & meshName, const Vector4i & colorV);
		void set_default_color(const Vector4i & colorRGB);

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