// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Ray.hpp"
#include "Util.hpp"

#include <variant>
#include <vector>


namespace Raytracer {

struct HitRecord
{
	Pos3 pos;
	Vec3 normal;
	float t;
	bool frontFacing;

	void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
		frontFacing = ray.Dir().dot(outwardNormal) < 0.0f;
		normal = frontFacing ? outwardNormal : -outwardNormal;
	}
};

class Sphere
{
public:
	Sphere() = delete;
	Sphere(Pos3 center, float radius) : m_center(std::move(center)), m_radius(radius) {}

	bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

private:
	Pos3 m_center;
	float m_radius;
};

#define ADD_SHAPE(Type, Member)                                                                \
public:                                                                                        \
	void Add(const std::initializer_list<Type>& obj)                                           \
	{                                                                                          \
		(Member).insert((Member).end(), obj.begin(), obj.end());                               \
	}                                                                                          \
	void Add(const Type& obj)                                                                  \
	{                                                                                          \
		(Member).push_back(obj);                                                               \
	}                                                                                          \
	template<typename T> requires std::is_same_v<T, Type>                                      \
	void Clear() { (Member).clear(); }                                                                                          \
	template<typename T> requires std::is_same_v<T, Type>                                      \
	[[nodiscard]] const std::vector<Type>& Get() const { return (Member); }                    \
                                                                                               \
private:                                                                                       \
	std::vector<Type> (Member)


class HittableList
{
public:
	HittableList() = default;

	bool Hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const;

	ADD_SHAPE(Sphere, m_spheres);
};

}  // namespace Raytracer
