// Copyright 2021 <github.com/razaqq>
#pragma once

// ReSharper disable CppLocalVariableMayBeConst
#include "Ray.hpp"
#include "Util.hpp"


namespace Raytracer {

class Camera
{
public:
	Camera(const Vec3& from, const Vec3& at, const Vec3& vUp,
		float hFov, float aspectRatio, float aperture, float focusDist)
		: m_origin(from), m_lensRadius(aperture / 2)
	{
		float vpWidth = 2.0f * std::tan(DegToRad(hFov) / 2.0f);
		float vpHeight = vpWidth / aspectRatio;

		m_w = (from - at).normalized();
		m_u = (vUp.cross(m_w)).normalized();
		m_v = m_w.cross(m_u);
		
		m_horizontal = focusDist * vpWidth * m_u;
		m_vertical = focusDist * vpHeight * m_v;
		m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - focusDist * m_w;
	}

	[[nodiscard]] Ray GetRay(float s, float t) const
	{
		Vec3 random = m_lensRadius * RandomInUnitDiskVec3();
		Vec3 offset = m_u * random.x() + m_v * random.y();
		return { m_origin + offset,
			m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset };
	}

private:
	Pos3 m_origin;
	Pos3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
	Vec3 m_u, m_v, m_w;
	float m_lensRadius;

};

}  // namespace Raytracer
