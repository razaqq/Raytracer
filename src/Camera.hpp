// Copyright 2021 <github.com/razaqq>
#pragma once

// ReSharper disable CppLocalVariableMayBeConst
#include "Ray.hpp"
#include "Util.hpp"


namespace Raytracer {

class Camera
{
public:
	Camera(const Vec3& from, const Vec3& at, const Vec3& vUp, float hFov, float aspectRatio)
	: m_origin(from)
	{
		float vpWidth = 2.0f * std::tan(DegToRad(hFov) / 2.0f);
		float vpHeight = vpWidth / aspectRatio;

		Vec3 w = (from - at).normalized();
		Vec3 u = (vUp.cross(w)).normalized();
		Vec3 v = w.cross(u);

		constexpr float focalLength = 1.0f;
		m_horizontal = vpWidth * u;
		m_vertical = vpHeight * v;
		m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - w;
	}

	[[nodiscard]] Ray GetRay(float s, float t) const
	{
		return { m_origin, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin };
	}

private:
	Pos3 m_origin;
	Pos3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
};

}  // namespace Raytracer
