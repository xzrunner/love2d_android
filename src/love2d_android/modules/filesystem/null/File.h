#ifndef LOVE_FILESYSTEM_NULL_FILE_H
#define LOVE_FILESYSTEM_NULL_FILE_H

// LOVE
#include <filesystem/File.h>

// STD
#include <string>

#ifdef LOVE_WINDOWS
#include <fstream>
#elif defined LOVE_ANDROID_NATIVE
#include <android_native_app_glue.h>
#endif

namespace love
{
namespace filesystem
{
namespace null
{
#ifdef LOVE_ANDROID_NATIVE
	struct ResourceDescriptor
	{
		int32_t m_descriptor;
		off_t m_start;
		off_t m_length;
	};
#endif // LOVE_ANDROID_NATIVE

	class File : public love::filesystem::File
	{
	private:

		// filename
		std::string filename;

		bool bOpen;

#ifdef LOVE_WINDOWS
		std::ifstream m_inputStream;
		char* m_buffer;
#elif defined LOVE_ANDROID_NATIVE
		AAssetManager* m_assetManager;
		AAsset* m_asset;
#elif defined LOVE_ANDROID_JNI
		unsigned long m_nSize;
		unsigned char* m_buffer;
		unsigned long m_offset;
#endif

	public:

		/**
		* Constructs an File with the given source and filename.
		* @param source The source from which to load the file. (Archive or directory)
		* @param filename The relative filepath of the file to load from the source.
		**/
#ifdef LOVE_ANDROID_NATIVE
		File(std::string filename, android_app* pApplication);
#else
		File(std::string filename);
#endif
		virtual ~File();

		// Implements love::filesystem::File.
		bool open(Mode mode);
		bool close();
		int64 getSize();
		Data * read(int64 size = ALL);
		int64 read(void * dst, int64 size);
		bool write(const void * data, int64 size);
		bool write(const Data * data, int64 size = ALL);
		bool eof();
		int64 tell();
		bool seek(uint64 pos);
		Mode getMode();
		std::string getFilename() const;
		std::string getExtension() const;

	public:
		const void* bufferize();

	private:
		int64 getSizeOpened();

#ifdef LOVE_ANDROID_NATIVE
		ResourceDescriptor descript();
#endif // LOVE_ANDROID_NATIVE

	}; // File

} // null
} // filesystem
} // love

#endif // LOVE_FILESYSTEM_NULL_FILE_H
