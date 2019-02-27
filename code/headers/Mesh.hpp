#pragma once

#include "declarations/Internal.hpp"
#include "declarations/Containers.hpp"
#include "declarations/Math.hpp"

#include "Rasterizer.hpp"
#include "Color_Buffer_Rgba8888.hpp"

namespace przurro
{
	class Mesh
	{
	private:

		Point4f_Buffer  &	ovPositions;
		Vector4f_Buffer	&	ovNormals; // original vertices normals, 4th component must be set to '0.f'
		i_Buffer			triangleIndices;

		Point4f_Buffer	&	tvPositions;
		Vector4f_Buffer	&	tvNormals;
		CColor_Buff			tvColors; // transformed vertices colors

		Point4i_Buffer		displayVertices;

		Color				color; // Main color of the vertices

		size_t meshFirstIndex;
		size_t numberOfIndices;
		//std::vector<toolkit::Point2f>			texcoords;  // 'vt'

	private:

		String name;

		int cacheIndices[4];//Cache array to store the actual indices to this class buffers. It's necessary to add the 4 position because is required in the rasterizer
		
	public:

		Mesh(Point4f_Buffer & vertexBuffer, Vector4f_Buffer & normalBuffer, size_t nVertex, size_t meshFIndex, String & meshName);
		
		~Mesh()
		{
			triangleIndices.clear();

			tvPositions.clear();
			tvNormals.clear();
			tvColors.clear();
			
			displayVertices.clear();
		}

	public:
		void update(Matrix44f & cameraMatrix, Matrix44f & projectedTransform, vec4 & lightVector, float lightIntensity = 2.f, float ambientalIntensity = 0.7f);
		void draw(Rasterizer<Color_Buff> & rasterizer);

	public:

		bool is_frontface(const Point4f * const projected_vertices, const int * const indices);

		void set_color(const Vector4i & colorV)
		{
			color.set(colorV[X], colorV[Y], colorV[Z]);
		}

		void set_color(const Color & inputColor)
		{
			color = inputColor;
		}


	public: // Clipping

		int Mesh::clip_with_viewport_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices);
		int clip_with_line_2d(const Point4f * vertices, int * first_index, int * last_index, Point4f * clipped_vertices, float a, float b, float c);
		Point4f intersect_rect(float a, float b, float c, const Point4f & point0, const Point4f & point1);

	public:

		i_Buffer & get_original_indices()
		{
			return triangleIndices;
		}
		
	};
}