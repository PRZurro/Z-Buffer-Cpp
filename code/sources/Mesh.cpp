#include "Mesh.hpp"

namespace przurro
{
	Mesh::Mesh() 
		: name("undefined") 
	{
	}

	Mesh::Mesh(String inputName = "undefined")
		 : name(inputName) 
	{
	}

	void Mesh::update(Projection_Matrix3f & projectionM)
	{
		// Se crean las matrices de transformación:

		Scale_Matrix3f			scaling(scale);
		Rotation_Matrix3f		rotationX, rotationY, rotationZ;
		Translation_Matrix3f	translation(0, 0, -10);

		rotationX.set< Rotation_Matrix3f::AROUND_THE_X_AXIS >(rotation[X]);
		rotationY.set< Rotation_Matrix3f::AROUND_THE_Y_AXIS >(rotation[Y]);
		rotationZ.set< Rotation_Matrix3f::AROUND_THE_Y_AXIS >(rotation[Z]);

		// Creación de la matriz de transformación unificada:

		Transform_Matrix3f transformation = projectionM * translation * rotationX * rotationY * rotationZ *  scaling;

		// Se transforman todos los vértices usando la matriz de transformación resultante:

		for (size_t index = 0, number_of_vertices = attributes.transformedVertices.size(); index < number_of_vertices; index++)
		{
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:

			Point4f & vertex = attributes.transformedVertices[index] = Matrix44f(transformation) * Matrix41f(attributes.originalVertices[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:

			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}
	}

	void Mesh::draw(Rasterizer<Color_Buff> & rasterizer)
	{
		// Se convierten las coordenadas transformadas y proyectadas a coordenadas
			// de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
			// La coordenada Z se escala a un valor suficientemente grande dentro del
			// rango de int (que es lo que espera fill_convex_polygon_z_buffer).

		float widthHalf = rasterizer.get_color_buffer().get_width() / 2;
		float heightHalf = rasterizer.get_color_buffer().get_height() / 2;

		Scale_Matrix3f			scaling = Scale_Matrix3f(widthHalf, heightHalf, 100000000.f);
		Translation_Matrix3f    translation = Translation_Matrix3f(widthHalf, heightHalf, 0.f);
		Transform_Matrix3f		transformation = translation * scaling;

		for (size_t index = 0, number_of_vertices = attributes.transformedVertices.size(); index < number_of_vertices; index++)
		{
			attributes.displayVertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(attributes.transformedVertices[index]));
		}

		// Se borra el framebúffer y se dibujan los triángulos:

		rasterizer.clear();

		for (int * indices = attributes.originalIndices.data(), *end = indices + attributes.originalIndices.size(); indices < end; indices += 3)
		{
			if (is_frontface(attributes.transformedVertices.data(), indices))
			{
				// Se establece el color del polígono a partir del color de su primer vértice:

				rasterizer.set_color(attributes.originalColors[*indices]);

				// Se rellena el polígono:

				rasterizer.fill_convex_polygon_z_buffer(attributes.displayVertices.data(), indices, indices + 3);
			}
		}
	}

	bool Mesh::is_frontface(const Point4f * const projected_vertices, const int * const indices)
	{
		const Point4f & v0 = projected_vertices[indices[0]];
		const Point4f & v1 = projected_vertices[indices[1]];
		const Point4f & v2 = projected_vertices[indices[2]];

		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}
}