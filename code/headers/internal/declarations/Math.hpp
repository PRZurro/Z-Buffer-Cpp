#pragma once

#include <Point.hpp>
#include <Translation.hpp>
#include <Rotation.hpp>
#include <Scaling.hpp>
#include <Projection.hpp>
#include <Vector.hpp>

#include <glm.hpp>

using namespace glm;

namespace przurro
{
	using Point2f = toolkit::Point2f;
	using Point4f = toolkit::Point4f;
	using Point4i = toolkit::Point4i;

	using Vector3f = toolkit::Vector3f;
	using Vector4f = toolkit::Vector4f;
	using Vector3i = toolkit::Vector3i;
	using Vector4i = toolkit::Vector4i;

	using Matrix44f = toolkit::Matrix44f;
	using Matrix41f = toolkit::Matrix41f;

	using Transform_Matrix3f = toolkit::Transformation3f;
	using Translation_Matrix3f = toolkit::Translation3f;
	using Rotation_Matrix3f = toolkit::Rotation3f;
	using Scale_Matrix3f = toolkit::Scaling3f;

	using Projection_Matrix3f = toolkit::Projection3f;

}