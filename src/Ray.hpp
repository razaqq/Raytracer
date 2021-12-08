// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Util.hpp"


namespace Raytracer {

class Ray
{
public:
	Ray() = default;
	Ray(Pos3 origin, Vec3 dir) : m_origin(std::move(origin)), m_dir(std::move(dir)) {}

	[[nodiscard]] Pos3 Origin() const { return m_origin; }
	[[nodiscard]] Vec3 Dir() const { return m_dir; }

	Pos3 At(float t) const
	{
		return m_origin + t * m_dir;  // P(t) = A * t * b
	}

private:
	Pos3 m_origin;
	Vec3 m_dir;
};

}  // namespace Raytracer
