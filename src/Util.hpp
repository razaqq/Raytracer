// Copyright 2021 <github.com/razaqq>
#pragma once

#include <Eigen/Core>

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

}  // namespace Raytracer

