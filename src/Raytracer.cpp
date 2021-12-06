// Copyright 2021 <github.com/razaqq>

#include "Log.hpp"
#include "Renderer.hpp"


int main(int argc, char* argv[])
{
	Raytracer::Log::Init();
	Raytracer::Renderer r;
	r.SetImageSize(1080, 1000 * 9/16);
	r.OutputImage("image.ppm");
	return 0;
}
