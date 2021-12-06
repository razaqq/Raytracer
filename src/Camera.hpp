// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Ray.hpp"
#include "Util.hpp"


namespace Raytracer {

class Camera
{
public:
	Camera(float width, float height)
	: m_origin({ 0.0f, 0.0f, 0.0f })
	{
		constexpr float vpHeight = 2.0f;
		const float vpWidth = width / height * vpHeight;
		constexpr float focalLength = 1.0f;
		m_horizontal = { vpWidth, 0.0f, 0.0f };
		m_vertical = { 0.0f, vpHeight, 0.0f };
		m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - Vec3{ 0.0f, 0.0f, focalLength };

	}

	[[nodiscard]] Ray GetRay(float u, float v) const
	{
		return { m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin };
	}

private:
	Pos3 m_origin;
	Pos3 m_lowerLeftCorner;
	Vec3 m_horizontal;
	Vec3 m_vertical;
};

}  // namespace Raytracer
