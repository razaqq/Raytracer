// Copyright 2021 <github.com/razaqq>

// ReSharper disable CppLocalVariableMayBeConst
#include "Material.hpp"

#include "HitRecord.hpp"
#include "Util.hpp"
#include "Ray.hpp"


using namespace Raytracer;

namespace {

static Vec3 Reflect(const Vec3& v, const Vec3& n)
{
	return v - 2.0f * v.dot(n) * n;
}

static Vec3 Refract(const Vec3& v, const Vec3& n, float refractRatio)
{
	float cosTheta = std::fmin((-v).dot(n), 1.0f);
	Vec3 rOutPerp = refractRatio * (v + cosTheta * n);
	Vec3 rOutParallel = -std::sqrt(std::fabs(1.0f - rOutPerp.squaredNorm())) * n;
	return rOutPerp + rOutParallel;
}

static float SchlickReflectance(float cos, float refractionIndex)
{
	// https://en.wikipedia.org/wiki/Schlick%27s_approximation

	float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * std::powf(1.0f - cos, 5.0f);
}

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
	Vec3 reflectedDir = Reflect(ray.Dir().normalized(), record.normal);

	attenuation = record.material->color;
	scattered = Ray(record.pos, reflectedDir + record.material->fuzz * RandomVec3());

	return scattered.Dir().dot(record.normal) > 0.0f;
}

static bool ScatterDielectric(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered)
{
	attenuation = { 1.0f, 1.0f, 1.0f };
	float refractRatio = record.frontFacing ? (1.0f / record.material->refractionIndex) : record.material->refractionIndex;
	Vec3 uDir = ray.Dir().normalized();

	float cosTheta = std::fmin((-uDir).dot(record.normal), 1.0f);
	float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

	Vec3 dir;
	if (refractRatio * sinTheta > 1.0f || SchlickReflectance(cosTheta, refractRatio) > RandomFloat())
	{
		dir = Reflect(uDir, record.normal);
	}
	else
	{
		dir = Refract(uDir, record.normal, refractRatio);
	}

	scattered = Ray(record.pos, dir);

	return true;
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
		case MaterialType::Dielectric:
			return ScatterDielectric(ray, record, attenuation, scattered);
	}
	return false;
}
