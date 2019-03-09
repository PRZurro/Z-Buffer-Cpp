#pragma once

#include "declarations/Internal.hpp"
#include "declarations/Math.hpp"
#include "declarations/Containers.hpp"


namespace przurro
{
	class Math
	{
	public:

		static bool is_frontface(const Point4f * const projected_vertices, const int * const indices)
		{
			const Point4f & v0 = projected_vertices[indices[X]];
			const Point4f & v1 = projected_vertices[indices[Y]];
			const Point4f & v2 = projected_vertices[indices[Z]];

			// Se asumen coordenadas proyectadas y pol�gonos definidos en sentido horario.+
			// Se comprueba a qu� lado de la l�nea que pasa por v0 y v1 queda el punto v2:

			return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
		}

		// otvFirst and Last: Original Transformed Vertices first and last elements; cvFirst, avFirst and ciFirst: clipped vertices, auxiliar vertices and clipped indices first elements
		static int clip_with_planes(Point4f * vertices, Vector4f_Buffer & frustumPlanes, Point4f_Buffer & transformedVertexPositions, TriangleI_Buffer & triangleBuffer, const size_t index)
		{
			constexpr size_t capacity = 20;

			static Point4f clippedVertices[capacity], auxVertices[capacity];
			clippedVertices[X] = vertices[X], clippedVertices[Y] = vertices[Y], clippedVertices[Z] = vertices[Z];

			int clippedIndices[capacity], clippedVerticesN = 0;
			int currVerticesN = 3;

			for (size_t i = 0; i < capacity; ++i)
				clippedIndices[i] = i; // {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19}

			for (Vector4f & plane : frustumPlanes)
			{
				copy(clippedVertices, clippedVertices + currVerticesN, auxVertices, auxVertices + currVerticesN);

				currVerticesN = clip_with_plane(auxVertices, clippedVertices, clippedIndices, clippedIndices + currVerticesN, plane);

				if (currVerticesN < 3)
					return currVerticesN;
			}

			int * currIndex = clippedIndices, *lastIndex = clippedIndices + currVerticesN;
			{ // pushing/editing clipped vertices to the input transformed vertex positions buffer
				for (size_t i = index, limit = index + 3; i < limit; ++i, ++currIndex)
				{
					transformedVertexPositions[i] = clippedVertices[*currIndex];
					*currIndex = i;
				}
				for (currIndex; currIndex < lastIndex; ++currIndex)
				{
					transformedVertexPositions.push_back(clippedVertices[*currIndex]);
					*currIndex = transformedVertexPositions.size() - 1;
				}
			}
			triangulate_polygon(clippedIndices, clippedIndices + currVerticesN, triangleBuffer);

			return currVerticesN;
		}

		static int clip_with_plane(Point4f * vertices, Point4f * outputVertices, int * firstIndex, int * lastIndex, const Vector4f & plane)
		{
			Point4f currVertex, nextVertex;

			int currentValue = 0, nextValue = 0, clippedVertexN = 0;
			float a = plane[X], b = plane[Y], c = plane[Z], d = plane[W];

			for (int * index = firstIndex; index < lastIndex; )
			{
				currVertex = vertices[*(index++)];

				if (index < lastIndex)
					nextVertex = vertices[*(index)];
				else nextVertex = vertices[*(firstIndex)];

				//In which side are on the evaluated vertices?
				currentValue =
					a * currVertex[0] + b * currVertex[1] + c * currVertex[2] + d > 0;
				nextValue =
					a * nextVertex[0] + b * nextVertex[1] + c * nextVertex[2] + d > 0;

				switch ((currentValue << 1) | nextValue) // Depending of the current sides of the evaluated vertices...
				{
				case 1:	// First outside and second inside
					outputVertices[clippedVertexN++] = intersect_plane(plane, currVertex, nextVertex);
					outputVertices[clippedVertexN++] = nextVertex;
					break;
				case 2:	// First inside and second outside
					outputVertices[clippedVertexN++] = intersect_plane(plane, currVertex, nextVertex);
					break;
				case 3:	// Both inside
					outputVertices[clippedVertexN++] = nextVertex;
					break;
				}
			}

			return clippedVertexN;
		}

		static Point4f intersect_plane(const Vector4f & plane, const Point4f & point0, const Point4f & point1)
		{
			float
				A = plane[X], B = plane[Y], C = plane[Z], D = plane[W],

				X0 = point0[X], Y0 = point0[Y], Z0 = point0[Z], W0 = point0[W],
				X1 = point1[X], Y1 = point1[Y], Z1 = point1[Z], W1 = point1[W],

				ß0 = X1 - X0, ß1 = Y1 - Y0, ß2 = Z1 - Z0, ß3 = W1 - W0,

				t = (D - (A * X0 + B * Y0 + C * Z0))
				/
				((A * ß0) + (B * ß1) + (C * ß2)),

				x = X0 + (t * ß0),
				y = Y0 + (t * ß1),
				z = Z0 + (t * ß2),
				w = W0 + (t * ß3); // Not sure if this is correct, but at least is not 0.000000

			return Point4f{ {x, y, z, w} };
		}

		static void triangulate_polygon(int * firstI, int * lastI, TriangleI_Buffer & triangleIndices)
		{
			for (int * i1 = firstI + Y, *i2 = firstI + Z; i2 < lastI;)
			{
				triangleIndices.push_back(Triangle_Index(*firstI, *(i1++), *(i2++)));
			}
		}
	};
	
}