#include "Mesh.hpp"
#include "Math_Functions.hpp" //Special math functions (like_clip_with_planes) at headers/internal/Math_Functions.hpp

namespace przurro
{
	Mesh::Mesh(Point4f_Buffer & vertexBuffer, Point4f_Buffer & transformedVertexBuffer, Vector4f_Buffer & normalBuffer, Vector4f_Buffer & transformedNormalBuffer, size_t nVertices, size_t meshFIndex, String & meshName)
		: name(meshName),
		ovPositions(vertexBuffer), 
		ovNormals(normalBuffer), 
		tvPositions(transformedVertexBuffer),
		tvNormals(transformedNormalBuffer),
		tvColors(nVertices),
		displayVerticesPositions(nVertices),
		fIndex(meshFIndex),
		lIndex(fIndex + nVertices - 1),
		color({0, 0, 0}),
		cacheIndices(4)
	{}

	void Mesh::update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & fPlanes)
	{
		displayTriangleIndices.clear();
		displayVerticesPositions.clear();

		// VERTEX LIGHT CALCULATION, BACKFACE CULLING, VERTICES CLIPPING WITH FRUSTUM PLANES
		{
			Color * vertexColor = tvColors.data();
			for (size_t index = fIndex; index < lIndex; index += 3)
			{
				// Vertices light calculation
				scale_color(vertexColor++, lightIntensities[index + X]);
				scale_color(vertexColor++, lightIntensities[index + Y]);
				scale_color(vertexColor++, lightIntensities[index + Z]);

				//---------------------------------------Back face Clipping-------------------------------------------------------------

				cacheIndices[X] = index; cacheIndices[Y] = index + Y; cacheIndices[Z] = index + Z;
				if (Math::is_frontface(tvPositions.data(), cacheIndices.data()))
				{
					int clippedVerticesN = Math::clip_with_planes(tvPositions.data() + index, fPlanes, tvPositions, displayTriangleIndices, index); // We clip each triangle with the frustum volume

					//displayTriangleI.push_back(Triangle_Index(index, index + Y, index + Z));
				}
			}
		}
	
		// NDC COORDINATES AND VIEWPORT COORDINATES
		{
			displayVerticesPositions.resize(displayTriangleIndices.size() * 3);

			float widthHalf = (float)rasterizer.get_color_buffer().get_width() / 2,
				heightHalf = (float)rasterizer.get_color_buffer().get_height() / 2;

			Transform_Matrix3f	viewportTransformation = Translation_Matrix3f(widthHalf, heightHalf, 0.f) * Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);

			for (size_t index = 0, nTriangles = displayTriangleIndices.size(); index < nTriangles; ++index)
			{
				Triangle_Index triangle = displayTriangleIndices[index];
				Point4f & tvp0 = tvPositions[triangle.v0], &tvp1 = tvPositions[triangle.v1], &tvp2 = tvPositions[triangle.v2];

				//---------------------------------------NDC Coordinates------------------------------------------------------
				float oneByW0 = (1.f / tvp0[W]), oneByW1 = (1.f / tvp1[W]), oneByW2 = (1.f / tvp2[W]);

				//Final vertex positions
				Point4f fvp0({ tvp0[X] * oneByW0, tvp0[Y] * oneByW0, tvp0[Z] * oneByW0, 1.f }),
					fvp1({ tvp1[X] * oneByW1, tvp1[Y] * oneByW1, tvp1[Z] * oneByW1, 1.f }),
					fvp2({ tvp2[X] * oneByW2, tvp2[Y] * oneByW2, tvp2[Z] * oneByW2, 1.f });

				//--------------------------------------Viewport Coordinates + Display Vertices Assignation-------------------

				size_t linearIndex = index * 3;

				Matrix41f temp = Matrix44f(viewportTransformation) * Matrix41f(fvp0);

				Point4i & dvp0 = displayVerticesPositions[linearIndex + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp0));
				Point4i & dvp1 = displayVerticesPositions[linearIndex + Y] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp1));
				Point4i & dvp2 = displayVerticesPositions[linearIndex + Z] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp2));

				displayTriangleIndices[index].set_indices_to_colors(triangle.v0, triangle.v1, triangle.v2);
				displayTriangleIndices[index].set_indices(linearIndex + X, linearIndex + Y, linearIndex + Z);
			}
		}
		
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		//The triangles are drawn

		for (Triangle_Index triangle : displayTriangleIndices)
		{
			cacheIndices[X] = triangle.v0; cacheIndices[Y] = triangle.v1; cacheIndices[Z] = triangle.v2; 

			rasterizer.set_color(tvColors[triangle.vColor0]); //The color of the polygon is established from the color of its first vertex
			rasterizer.fill_convex_polygon_z_buffer(displayVerticesPositions.data(), cacheIndices.data(), cacheIndices.data() + W); //The polygon is filled
		}
	}
}