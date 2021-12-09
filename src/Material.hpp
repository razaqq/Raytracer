// Copyright 2021 <github.com/razaqq>
#pragma once

#include "HitRecord.hpp"
#include "Util.hpp"
#include "Ray.hpp"


namespace Raytracer {

enum class MaterialType
{
	Lambertian,
	Metal,
	Dielectric,
};

struct Material
{
	MaterialType type;
	Color color;

	// for metals
	float fuzz = 0.0f;

	// for dielectrics
	float refractionIndex = 0.0f;
};

bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered);

}  // namespace Raytracer
