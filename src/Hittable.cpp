// Copyright 2021 <github.com/razaqq>

#include "Hittable.hpp"

#include "Ray.hpp"
#include "Util.hpp"


using Raytracer::HittableList;
using Raytracer::Sphere;

bool Sphere::Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	// (P(t) - C) * (P(t) - C) = r^2
	// Ray has form P(t) = A + t * b, C is center of the circle, r is radius
	// t^2 * b^2 + 2tb * (A - C) + (A - C) * (A - C) - r^2 = 0
	// --> Point P is on the sphere

	Vec3 directLine = ray.Origin() - m_center;  // (A - C)
	float d2 = ray.Dir().squaredNorm();  // |d|^2

	float a = directLine.dot(ray.Dir());  // b * (A - C)
	float b = directLine.squaredNorm() - m_radius * m_radius;  // |(A - C)|^2 - r^2
	float discriminant = a * a - d2 * b;  // D = b^2 - 4ac

	// D > 0: polynomial has two distinct real roots --> has to intersect the circle twice
	// D = 0: sqrt is zero --> has to intersect the circle once
	// D < 0: no solution for real numbers --> no intersection

	if (discriminant < 0.0f)
		return false;

	float sqrtDis = std::sqrt(discriminant);
	float root = (-a - sqrtDis) / d2;

	auto isValid = [tMin, tMax](float root) -> bool
	{
		return root < tMin || tMax < root;
	};

	if (isValid(root))
	{
		root = (-a + sqrtDis) / d2;
		if (isValid(root))
		{
			return false;
		}
	}

	record.t = root;
	record.pos = ray.At(root);
	record.SetFaceNormal(ray, (record.pos - m_center) / m_radius);
	record.material = &const_cast<Material&>(m_material);

	return true;
}

bool HittableList::Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	HitRecord tempRecord;
	bool hit = false;
	float closest = tMax;

#define CHECK_HIT(Member)                                 \
	for (const auto& hittable : (Member))                 \
	{                                                     \
		if (hittable.Hit(ray, tMin, closest, tempRecord)) \
		{                                                 \
			hit = true;                                   \
			record = tempRecord;                          \
			closest = record.t;                           \
		}                                                 \
	}

	CHECK_HIT(m_spheres)

	return hit;
}
