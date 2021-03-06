// Copyright 2021 <github.com/razaqq>
#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <numbers>
#include <random>
#include <vector>

namespace Raytracer {

using Buffer = std::vector<uint8_t>;
using Color = Eigen::Vector3f;
using Pos3 = Eigen::Vector3f;
using Vec3 = Eigen::Vector3f;

static constexpr float Infinity = std::numeric_limits<float>::infinity();
static constexpr float Pi = std::numbers::pi_v<float>;

inline float DegToRad(float deg)
{
	return deg * Pi / 180.0f;
}

static std::mt19937 g_pcg;

inline float RandomFloat(float min = 0.0f, float max = 1.0f)
{
	return std::uniform_real_distribution(min, max)(g_pcg);
}

inline Color RandomColor(float min, float max)
{
	return { RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max) };
}

// RandomVec3, RandomUnitVec3 and RandomInHemisphereVec3
// can be used as different diffuse renderers
inline Vec3 RandomVec3(float maxLength = 1.0f)
{
	while (true)
	{
		Vec3 v = Vec3::Random();
		float length = v.squaredNorm();
		if (length >= maxLength) continue;
		return v;
	}
}

inline Vec3 RandomUnitVec3()
{
	return RandomVec3().normalized();
}

inline Vec3 RandomInHemisphereVec3(const Vec3& normal)
{
	Vec3 r = RandomVec3();
	// same hemisphere as normal
	return r.dot(normal) > 0.0f ? r : -r;
}

inline Vec3 RandomInUnitDiskVec3()
{
	while(true)
	{
		Vec3 p = { RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f };
		if (p.squaredNorm() >= 1.0f) continue;
		return p;
	}
}

}  // namespace Raytracer
