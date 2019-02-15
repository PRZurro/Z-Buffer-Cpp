#pragma once

#include "declarations/Internal.hpp"
#include "declarations/Containers.hpp"
#include "declarations/Math.hpp"
#include "declarations/External_Tools.hpp"

#include "Rasterizer.hpp"
#include "Color_Buffer_Rgba8888.hpp"

#include <tiny_obj_loader.h>

namespace przurro
{
	class Mesh
	{
		typedef struct Mesh_Attributes
		{
			Point4f_Buffer  *	ovPositions;
			Vector4f_Buffer	*	ovNormals; // original vertices normals, 4th component must be set to '0.f'
			Color_Buff			ovColors; // original vertices colors

			i_Buffer			originalIndices;

			Point4f_Buffer		transformedVertices;
			Point4i_Buffer		displayVertices;

			Color				color; // Main color of the vertices

			~Mesh_Attributes()
			{
				transformedVertices.clear();
				displayVertices.clear();
			}
			//std::vector<toolkit::Point2f>			texcoords;  // 'vt'
		};

		using Mesh_Attributes_sptr = std::shared_ptr<Mesh_Attributes>;

	private:

		String name;
		
	private:

		Mesh_Attributes_sptr attributes;

	public:

		Mesh(Point4f_Buffer * vertexBuffer, Vector4f_Buffer * normalBuffer);

	public:

		void update(Transform_Matrix3f modelTransform);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);

		void set_color(Vector3f colorV)
		{
			attributes->color.set(colorV[X], colorV[Y], colorV[Z]);
		}

	public:

		i_Buffer & get_original_indices()
		{
			return attributes->originalIndices;
		}
		
	};
}