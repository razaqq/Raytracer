// Copyright 2021 <github.com/razaqq>
#pragma once

#include "Util.hpp"

#include <string>
#include <vector>


namespace Raytracer {

class Renderer
{
public:
	void SetImageSize(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
	}
	void OutputImage(std::string_view fileName) const;

private:
	uint32_t m_width = 0;
	uint32_t m_height = 0;

};

}  // namespace Raytracer
