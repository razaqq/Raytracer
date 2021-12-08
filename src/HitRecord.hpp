// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Ray.hpp"
#include "Util.hpp"


namespace Raytracer {

struct Material;

struct HitRecord
{
	Pos3 pos;
	Vec3 normal;
	float t;
	bool frontFacing;
	Material* material;

	void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
		frontFacing = ray.Dir().dot(outwardNormal) < 0.0f;
		normal = frontFacing ? outwardNormal : -outwardNormal;
	}
};

}  // namespace Raytracer
