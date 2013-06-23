#ifndef LOVE_IMAGE_NULL_IMAGE_H
#define LOVE_IMAGE_NULL_IMAGE_H

// LOVE
#include <image/Image.h>

namespace love
{
namespace image
{
namespace null
{
	class Image : public love::image::Image
	{
	public:

		Image();
		 ~Image();

		// Implements Module.
		const char * getName() const;

		love::image::ImageData * newImageData(love::filesystem::File * file);
		love::image::ImageData * newImageData(Data * data);
		love::image::ImageData * newImageData(int width, int height);
		love::image::ImageData * newImageData(int width, int height, void *data);

	}; // Image

} // null
} // image
} // love

#endif // LOVE_IMAGE_NULL_IMAGE_H