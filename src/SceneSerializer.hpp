// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Hittable.hpp"


namespace Raytracer {

inline HittableList RandomScene()
{
	HittableList world;

	Material ground{ MaterialType::Lambertian, { 0.5f, 0.5f, 0.5f } };
	world.Add(Sphere({ 0.0f, -1000.0f, 0.0f }, 1000.0f, ground));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float rng = RandomFloat();

			Vec3 center{ a + 0.9f * RandomFloat(), 0.2f, b + 0.9f * RandomFloat() };

			if ((center - Vec3{ 4.0f, 0.2f, 0.0f }).norm() <= 0.9f)
			{
				continue;
			}

			Material m;
			if (rng < 0.8f)
			{
				m = { MaterialType::Lambertian, Color::Random().array() * Color::Random().array() };
			}
			else if (rng < 0.95f)
			{
				m = { MaterialType::Metal, RandomColor(0.5f, 1.0f), RandomFloat(0.0f, 0.5f) };
			}
			else
			{
				m = { MaterialType::Dielectric, {}, 0.0f, 1.5f };
			}
			world.Add(Sphere(center, 0.2f, m));
		}
	}

	Material m1{ MaterialType::Dielectric, {}, 0.0f, 1.5f };
	Material m2{ MaterialType::Lambertian, { 0.4f, 0.2f, 0.1f } };
	Material m3{ MaterialType::Metal, { 0.7f, 0.6f, 0.5f }, 0.0f };

	world.Add({
		{ Sphere({ 0.0f, 1.0f, 0.0f }, 1.0f, m1) },
		{ Sphere({ -4.0f, 1.0f, 0.0f }, 1.0f, m2) },
		{ Sphere({ 4.0f, 1.0f, 0.0f }, 1.0f, m3) }
	});

	return world;
}

}  // namespace Raytracer
