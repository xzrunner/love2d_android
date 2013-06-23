#include "Image.h"

#include "ImageData.h"

namespace love
{
namespace image
{
namespace null
{
	Image::Image()
	{
	}

	Image::~Image()
	{
	}

	const char * Image::getName() const
	{
		return "love.image.null";
	}

	love::image::ImageData * Image::newImageData(love::filesystem::File * file)
	{
		return new ImageData(file);
	}

	love::image::ImageData * Image::newImageData(Data * data)
	{
//		return new ImageData(data);
		return NULL;
	}

	love::image::ImageData * Image::newImageData(int width, int height)
	{
		return new ImageData(width, height);
	}

	love::image::ImageData * Image::newImageData(int width, int height, void *data)
	{
		return new ImageData(width, height, data);
	}

} // null
} // image
} // love
