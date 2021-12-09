// Copyright 2021 <github.com/razaqq>

// ReSharper disable CppLocalVariableMayBeConst
#include "Renderer.hpp"

#include "Camera.hpp"
#include "Hittable.hpp"
#include "Log.hpp"
#include "Material.hpp"
#include "Ray.hpp"

#include <algorithm>
#include <fstream>
#include <vector>


using namespace Raytracer;

namespace {

static void WriteColor(std::ostream& stream, const Color& color, float samplesPerPixel)
{
	// sqrt for of gamma 2
	float r = std::clamp(std::sqrt(color.x() / samplesPerPixel), 0.0f, 0.999f);
	float g = std::clamp(std::sqrt(color.y() / samplesPerPixel), 0.0f, 0.999f);
	float b = std::clamp(std::sqrt(color.z() / samplesPerPixel), 0.0f, 0.999f);

	stream << static_cast<int>(256 * r) << ' '
		   << static_cast<int>(256 * g) << ' '
		   << static_cast<int>(256 * b) << '\n';
}

static Color RayColor(const Ray& ray, const HittableList& world, uint32_t depth)
{
	Ray currentRay = ray;
	Color currentAttenuation = {1.0f, 1.0f, 1.0f};

	for (size_t i = 0; i < depth; i++)
	{
		HitRecord record;
		if (world.Hit(currentRay, 0.001f, Infinity, record))
		{
			Ray scattered;
			Vec3 attenuation;
			assert(record.material);
			if (Scatter(currentRay, record, attenuation, scattered))
			{
				currentAttenuation = currentAttenuation.array() * attenuation.array();
				currentRay = scattered;
			}
			else
			{
				return { 0.0f, 0.0f, 0.0f };
			}
		}
		else
		{
			Vec3 uDir = currentRay.Dir().normalized();
			float t = 0.5f * (uDir.y() + 1.0f);
			Color c = (1.0f - t) * Color{ 1.0f, 1.0f, 1.0f } + t * Color{ 0.5f, 0.7f, 1.0f };
			return currentAttenuation.array() * c.array();
		}
	}
	return { 0.0f, 0.0f, 0.0f };
}

}  // namespace


void Renderer::OutputImage(std::string_view fileName) const
{
	constexpr uint32_t maxDepth = 8;
	constexpr uint32_t samplesPerPixel = 32;

	HittableList world;
	world.Add({
		{ Sphere({ 0.0f, -100.5f, -1.0f }, 100.0f, { MaterialType::Lambertian, { 0.8f, 0.8f, 0.0f } }) },
		{ Sphere({ 0.0f, 0.0f, -1.0f },    0.5f,   { MaterialType::Lambertian, { 0.1f, 0.2f, 0.5f } }) },
		//{ Sphere({ 0.0f, 0.0f, -1.0f },    0.5f,   { MaterialType::Lambertian, { 0.7f, 0.3f, 0.3f } }) },
		{ Sphere({ -1.0f, 0.0f, -1.0f },   0.5f,   { MaterialType::Dielectric,      { 0.8f, 0.8f, 0.8f }, 1.0f, 1.5f }) },
		// { Sphere({ -1.0f, 0.0f, -1.0f },   -0.45f,   { MaterialType::Dielectric,      { 0.8f, 0.8f, 0.8f }, 1.0f, 1.5f }) },
		//{ Sphere({ -1.0f, 0.0f, -1.0f },   0.5f,   { MaterialType::Metal,      { 0.8f, 0.8f, 0.8f }, 0.3f }) },
		{ Sphere({ 1.0f, 0.0f, -1.0f },    0.5f,   { MaterialType::Metal,      { 0.8f, 0.6f, 0.2f }, 0.0f }) }
	});

	auto fWidth = static_cast<float>(m_width);
	auto fHeight = static_cast<float>(m_height);

	// Camera
	Camera camera({ 1, 1, 1 }, { 0, 0, -1 }, { 0, 1, 0 } , 90, fWidth / fHeight);

	// Create the image
	std::ofstream stream;
	stream.open(fileName.data());

	stream << "P3\n" << m_width << ' ' << m_height << "\n255\n";

	for (int j = m_height - 1; j >= 0; j--)
	{
		for (int i = 0; i < m_width; i++)
		{
			Color color{ 0.0f, 0.0f, 0.0f };

			for (int s = 0; s < samplesPerPixel; s++)
			{
				float u = (static_cast<float>(i) + RandomFloat()) / (fWidth - 1.0f);
				float v = (static_cast<float>(j) + RandomFloat()) / (fHeight - 1.0f);

				color += RayColor(camera.GetRay(u, v), world, maxDepth);
			}

           WriteColor(stream, color, samplesPerPixel);
		}
	}

	stream.flush();
	stream.close();
}
