#ifndef LOVE_NULL_IMAGE_DATA_H
#define LOVE_NULL_IMAGE_DATA_H

// GL
#include <GLES/gl.h>

// LOVE
#include <filesystem/File.h>
#include <image/ImageData.h>

namespace love
{
namespace image
{
namespace null
{
	class ImageData : public love::image::ImageData
	{
	public:
		enum Type
		{
			e_unknown = 0,
			e_png,
			e_jpg
		};

	private:

		// Create imagedata. Initialize with data if not null.
		void create(int width, int height, void * data = 0);

	public:

		ImageData(love::filesystem::File * file);
		ImageData(int width, int height);
		ImageData(int width, int height, void *data);
		virtual ~ImageData();

		// Implements ImageData.
		void encode(love::filesystem::File * f, Format format);

		Type getType() const { return m_type; }

		GLint getFormat() { return m_format; }

		const std::string& getFilepath() const { return m_filepath; }

		// Load an encoded format.
		void load();

	private:
		void loadByLibPng(void* data, int size);
		void loadByLibJpeg(void* data, int size); 

	private:
		bool m_noColor;

		Type m_type;

		GLint m_format;

		std::string m_filepath;

		love::filesystem::File* m_initFile;

	}; // ImageData

} // null
} // image
} // love

#endif // LOVE_NULL_IMAGE_DATA_H
