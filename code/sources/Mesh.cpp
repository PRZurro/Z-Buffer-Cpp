#include "Mesh.hpp"

namespace przurro
{
	Mesh::Mesh(Point4f_Buffer & vertexBuffer, Point4f_Buffer & transformedVertexBuffer, Vector4f_Buffer & normalBuffer, Vector4f_Buffer & transformedNormalBuffer, size_t nVertices, size_t meshFIndex, String & meshName)
		: name(meshName),
		ovPositions(vertexBuffer), 
		ovNormals(normalBuffer), 
		tvPositions(transformedVertexBuffer),
		tvNormals(transformedNormalBuffer),
		tvColors(nVertices),
		displayVertices(nVertices),
		fIndex(meshFIndex),
		lIndex(fIndex + nVertices - 1),
		color({0, 0, 0})
	{}

	void Mesh::update(f_Buffer & lightIntensities, Rasterizer<Color_Buff> & rasterizer, Vector4f_Buffer & frustrumPlanes)
	{
		Color * vertexColor = tvColors.data();
		for (size_t index = fIndex; index < lIndex; index += 3)
		{
			//---------------------------------------Triangle vertex colors----------------------------------------------
		
			scale_color(vertexColor++, lightIntensities[index + X]);
			scale_color(vertexColor++, lightIntensities[index + Y]);
			scale_color(vertexColor++, lightIntensities[index + Z]);
	
			//---------------------------------------Clipping-------------------------------------------------------------
			
			Point4f_Buffer clippedVertices({ tvPositions[index + X], tvPositions[index + Y], tvPositions[index + Z]});
			i_Buffer clippedIndices({(int)index + X, (int)index + Y, (int)index + Z});

			clip_with_viewport_3D(clippedVertices, clippedIndices, frustrumPlanes);
			triangulate_polygon(clippedIndices, displayTriangleI);
		}

		displayVertices.resize(tvPositions.size());

		float widthHalf = (float)rasterizer.get_color_buffer().get_width() / 2;
		float heightHalf = (float)rasterizer.get_color_buffer().get_height() / 2;

		Transform_Matrix3f	viewportTransformation = Translation_Matrix3f(widthHalf, heightHalf, 0.f) * Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);

		for (size_t index = 0, nTriangles = displayTriangleI.size(); index < nTriangles; ++index)
		{
			Point4f & tvp0 = tvPositions[displayTriangleI[index].v0], & tvp1 = tvPositions[displayTriangleI[index].v1], & tvp2 = tvPositions[displayTriangleI[index].v2];

			//---------------------------------------NDC Coordinates------------------------------------------------------
			float oneByW0 = (1.f / tvp0[W]), oneByW1 = (1.f / tvp1[W]), oneByW2 = (1.f / tvp2[W]);

			//Final vertex positions
			Point4f fvp0({ tvp0[X] * oneByW0, tvp0[Y] * oneByW0, tvp0[Z] * oneByW0, 1.f }),
					fvp1({ tvp1[X] * oneByW1, tvp1[Y] * oneByW1, tvp1[Z] * oneByW1, 1.f }),
					fvp2({ tvp2[X] * oneByW2, tvp2[Y] * oneByW2, tvp2[Z] * oneByW2, 1.f });

			//--------------------------------------Viewport Coordinates + Display Vertices Assignation-------------------

			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp0));
			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp1));
			displayVertices[index + X] = Point4i(Matrix44f(viewportTransformation) * Matrix41f(fvp2));
		}
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		// Se borra el frameb�ffer y se dibujan los tri�ngulos:

		rasterizer.clear();

		int cacheIndices[4];//Cache array to store the actual indices to this class buffers. It's necessary to add the 4 position because is required in the rasterizer
		cacheIndices[W] = sizeof(int);
		for (Triangle_Index triangle : displayTriangleI)
		{
			cacheIndices[X] = triangle.v0; cacheIndices[Y] = triangle.v1; cacheIndices[Z] = triangle.v2; 

			if (is_frontface(tvPositions.data(), cacheIndices))
			{
				rasterizer.set_color(tvColors[triangle.v0]); //The color of the polygon is established from the color of its first vertex
				rasterizer.fill_convex_polygon_z_buffer(displayVertices.data(), cacheIndices, cacheIndices + W); //The polygon is filled
			}
		}
	}

	bool Mesh::is_frontface(const Point4f * const projected_vertices, const int * const indices)
	{
		const Point4f & v0 = projected_vertices[indices[X]];
		const Point4f & v1 = projected_vertices[indices[Y]];
		const Point4f & v2 = projected_vertices[indices[Z]];

		// Se asumen coordenadas proyectadas y pol�gonos definidos en sentido horario.
		// Se comprueba a qu� lado de la l�nea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}

	int Mesh::clip_with_viewport_3D(Point4f_Buffer & clippedVertices,  i_Buffer & clippedIndices, const Vector4f_Buffer & frustrumPlanes)
	{
		int clippedVerticesN = 0, count = 0;

		Point4f_Buffer outputVertices = clippedVertices, auxVertices{};
		for (const Vector4f & plane : frustrumPlanes)
		{
			auxVertices = outputVertices;
			outputVertices.clear();

			clippedVerticesN = clip_with_plane_3D(auxVertices, outputVertices, plane);

			if (clippedVerticesN < 3)
				return 0;

			count += clippedVerticesN;
		}

		clippedVertices = outputVertices;
		return count;
	}
	
	int Mesh::clip_with_plane_3D(Point4f_Buffer & currentVertices, Point4f_Buffer & outputVertices, const Vector4f & plane)
	{
		Point4f currentVertex, nextVertex;

		int currentValue = 0, nextValue = 0, clippedVertexN = 0;
		float a = plane[X], b = plane[Y], c = plane[Z], d = plane[W];

		for (size_t i = 0, size = currentVertices.size(); i < size;)
		{
			currentVertex = currentVertices[i++];
			nextVertex = currentVertices[i];

			//In which side are on the evaluated vertices?
			currentValue =
				a * currentVertex[0] + b * currentVertex[1] + c * currentVertex[2] + d > 0;
			nextValue =
				a * nextVertex[0] + b * nextVertex[1] + c * nextVertex[2] + d > 0;

			switch ((currentValue << 1) | nextValue) // Depending of the current sides of the evaluated vertices...
			{
				case 1:	// First outside and second inside
					outputVertices.push_back(intersect_plane(plane, currentVertex, nextVertex));
					outputVertices.push_back(nextVertex);
					++clippedVertexN;
					break;
				case 2:	// First inside and second outside
					outputVertices.push_back(intersect_plane(plane, currentVertex, nextVertex));
					++clippedVertexN;
					break;
				case 3:	// Both inside
					outputVertices.push_back(nextVertex);
					break;
			}
		}

		return clippedVertexN;
	}

	Point4f Mesh::intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1)
	{
		float b0 = point1[X] - point0[X], b1 = point1[Y] - point0[Y], b2 = point1[Z] - point0[Z];

		float	t = - ((plane[X] * point0[X]) + (plane[Y] * point0[Y]) + (plane[Z] * point0[Z]) + plane[W]);
				t /= (b0 + b1 + b2);
		
		Point4f intersectionPoint
		{ {

				point0[X] + t * b0,
				point0[Y] + t * b1,
				point0[Z] + t * b2
		} };

		return intersectionPoint;
	}

	void triangulate_polygon(i_Buffer & indices, TriangleI_Buffer & triangleIndices)
	{
		for (size_t v0 = 0, tmpv1 = 1, tmpv2 = 2, size = indices.size(); tmpv2 < size; ++tmpv1, ++tmpv2)
		{
			triangleIndices.push_back(Triangle_Index(v0, tmpv1, tmpv2));
		}
	}
}