// Copyright 2021 <github.com/razaqq>

#include "Material.hpp"

#include "HitRecord.hpp"
#include "Util.hpp"
#include "Ray.hpp"


using namespace Raytracer;

namespace {

// diffuse materials
static bool ScatterLambertian(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered)
{
	Vec3 scatterDir = record.normal + RandomVec3();

	// make sure scatterDir is not zero
	if (scatterDir.isZero(1e-8f))
	{
		scatterDir = record.normal;
	}

	attenuation = record.material->color;
	scattered = Ray(record.pos, scatterDir);

	return true;
}

static bool ScatterMetal(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered)
{
	const Vec3 reflectedDir = Reflect(ray.Dir().normalized(), record.normal);

	attenuation = record.material->color;
	scattered = Ray(record.pos, reflectedDir + record.material->fuzz * RandomVec3());

	return scattered.Dir().dot(record.normal) > 0.0f;
}

}

bool Raytracer::Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered)
{
	switch (record.material->type)
	{
		case MaterialType::Lambertian:
			return ScatterLambertian(ray, record, attenuation, scattered);
		case MaterialType::Metal:
			return ScatterMetal(ray, record, attenuation, scattered);
	}
	return false;
}
