// Copyright 2021 <github.com/razaqq>

// ReSharper disable CppLocalVariableMayBeConst
#include "Renderer.hpp"

#include "Camera.hpp"
#include "Hittable.hpp"
#include "Log.hpp"
#include "Ray.hpp"

#include <algorithm>
#include <fstream>
#include <vector>


using Raytracer::Color;
using Raytracer::HittableList;
using Raytracer::HitRecord;
using Raytracer::Infinity;
using Raytracer::Pos3;
using Raytracer::Ray;
using Raytracer::Renderer;
using Raytracer::Vec3;

namespace {

static void WriteColor(std::ostream& stream, const Color& color, float samplesPerPixel)
{
	float r = std::clamp(color.x() / samplesPerPixel, 0.0f, 0.999f);
	float g = std::clamp(color.y() / samplesPerPixel, 0.0f, 0.999f);
	float b = std::clamp(color.z() / samplesPerPixel, 0.0f, 0.999f);

	stream << static_cast<int>(256 * r) << ' '
		   << static_cast<int>(256 * g) << ' '
		   << static_cast<int>(256 * b) << '\n';
}

static Color RayColor(const Ray& ray, const HittableList& world)
{
	HitRecord record;
	if (world.Hit(ray, 0.0f, Infinity, record))
	{
		return 0.5f * (record.normal + Color{ 1, 1, 1 });
	}

	Vec3 uDir = ray.Dir().normalized();
	float t = 0.5f * (uDir.y() + 1.0f);
	return (1.0f - t) * Color{ 1.0f, 1.0f, 1.0f } + t * Color{0.5f, 0.7f, 1.0f};
}

}  // namespace


void Renderer::OutputImage(std::string_view fileName) const
{
	constexpr uint32_t samplesPerPixel = 100;

	HittableList world;
	world.Add({
		{ Sphere({ 0.0f, 0.0f, -1.0f }, 0.5f) },
		{ Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f) }
	});

	auto fWidth = static_cast<float>(m_width);
	auto fHeight = static_cast<float>(m_height);

	// Camera
	Camera camera(fWidth, fHeight);

	// Create the image
	std::ofstream stream;
	stream.open(fileName.data());

	stream << "P3\n" << m_width << ' ' << m_height << "\n255\n";

	for (int j = m_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < m_width; i++)
		{
			Color color{ 0.0f, 0.0f, 0.0f };

			size_t s = 0;
			while (s < samplesPerPixel)
			{
				float u = (static_cast<float>(i) + RandomFloat()) / (fWidth - 1.0f);
				float v = (static_cast<float>(j) + RandomFloat()) / (fHeight - 1.0f);

				color += RayColor(camera.GetRay(u, v), world);
				s++;
			}

			WriteColor(stream, color, samplesPerPixel);
		}
	}

	stream.flush();
	stream.close();
}
