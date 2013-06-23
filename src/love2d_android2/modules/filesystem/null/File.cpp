#include "File.h"

// STD
#include <cstring>

// LOVE
#ifdef LOVE_ANDROID_JNI
#	include "Filesystem.h"
#endif // LOVE_ANDROID_JNI
#include <filesystem/FileData.h>

#include <sys/stat.h>

namespace love
{
namespace filesystem
{
namespace null
{
#ifdef LOVE_ANDROID_NATIVE
	File::File(std::string filename, android_app* pApplication)
		: filename(filename)
		, bOpen(false)
	{
		m_assetManager = pApplication->activity->assetManager;
		m_asset = NULL;
	}
#else
	File::File(std::string filename)
		: filename(filename)
		, bOpen(false)
		, m_buffer(NULL)
	{
#ifdef LOVE_ANDROID_JNI
		//	m_pFile = NULL;
		m_nSize = 0;
		m_offset = 0;
#endif // LOVE_ANDROID_JNI
	}
#endif

	File::~File()
	{
		if (bOpen)
			close();
	}

	bool File::open(Mode mode)
	{
		if (bOpen) return true;

#ifdef LOVE_WINDOWS
		m_inputStream.open(filename.c_str(), std::ios::in | std::ios::binary);
		bOpen = !m_inputStream.fail();
#elif defined LOVE_ANDROID_NATIVE
		m_asset = AAssetManager_open(m_assetManager, filename.c_str(), AASSET_MODE_UNKNOWN);
		bOpen = (m_asset != NULL);
#elif defined LOVE_ANDROID_JNI
		m_buffer = love::filesystem::null::Filesystem::Instance()->getFileData(filename.c_str(), "rb", &m_nSize);
		//	m_pFile = fmemopen(m_buffer, m_nSize, "rb");
		bOpen = true;
#endif
		return bOpen;
	}

	bool File::close()
	{
		if (!bOpen) return true;

		bOpen = false;
#ifdef LOVE_WINDOWS
		m_inputStream.close();
		delete[] m_buffer; m_buffer = NULL;
#elif defined LOVE_ANDROID_NATIVE
		if (m_asset != NULL)
		{
			AAsset_close(m_asset);
			m_asset = NULL;
		}
#elif defined LOVE_ANDROID_JNI
		// 	fclose(m_pFile);
		// 	m_pFile = NULL;
		delete[] m_buffer; m_buffer = NULL;
#endif
		return true;
	}

	int64 File::getSize()
	{
		if (!bOpen) 
		{
			if (!open(File::READ))
				throw love::Exception("Could not read file %s.", filename.c_str());
			int64 size = getSizeOpened();
			close();
			return size;
		}

		return getSizeOpened();
	}

	Data * File::read(int64 size/* = ALL*/)
	{
		bool isOpen = bOpen;
		
		if (!isOpen)
		{
			if (!open(File::READ))
				throw love::Exception("Could not read file %s.", filename.c_str());
		}

		int64 max = getSize();
		size = (size == ALL) ? max : size;
		size = (size > max) ? max : size;

		FileData* fileData = new FileData(size, getFilename());

		read(fileData->getData(), size);

		if (!isOpen)
			close();

		return fileData;
	}

	int64 File::read(void * dst, int64 size)
	{
		bool isOpen = bOpen;

		if (!isOpen)
		{
			if (!open(File::READ))
				throw love::Exception("Could not read file %s.", filename.c_str());
		}

		int64 max = getSize();
		size = (size == ALL) ? max : size;
		size = (size > max) ? max : size;

#ifdef LOVE_WINDOWS
		m_inputStream.read((char*)dst, size);
		size = (!m_inputStream.fail()) ? size : 0;
#elif defined LOVE_ANDROID_NATIVE
		int32_t lReadCount = AAsset_read(m_asset, dst, size);
		size (lReadCount == size) ? size : 0;
#elif defined LOVE_ANDROID_JNI
		if(m_offset + size <= m_nSize)
		{
			memcpy(dst, m_buffer + m_offset, size);
			m_offset += size;
		}
		else
		{
			size = 0;
		}
#endif

		if (!isOpen)
			close();

		return size;
	}

	bool File::write(const void * data, int64 size)
	{
		return false;
	}

	bool File::write(const Data * data, int64 size)
	{
		return false;
	}

	bool File::eof()
	{
		return true;
	}

	int64 File::tell()
	{
		return -1;
	}

	bool File::seek(uint64 pos)
	{
		return false;
	}

	filesystem::File::Mode File::getMode()
	{
		return bOpen ? READ : CLOSED;
	}

	std::string File::getFilename() const
	{
		return filename;
	}

	std::string File::getExtension() const
	{
		std::string::size_type idx = filename.rfind('.');

		if (idx != std::string::npos)
			return filename.substr(idx+1);
		else
			return std::string();
	}

	const void* File::bufferize()
	{
		bool isOpen = bOpen;

		if (!isOpen)
		{
			if (!open(File::READ))
				throw love::Exception("Could not read file %s.", filename.c_str());
		}

		void* ret;

#ifdef LOVE_WINDOWS
		off_t lSize = getSize();
		if (lSize <= 0) ret = NULL;

		if (!m_buffer)
		{
			m_buffer = new char[lSize];
			m_inputStream.read(m_buffer, lSize);
		}
		ret = m_inputStream.fail() ? NULL : m_buffer;
#elif defined LOVE_ANDROID_NATIVE
		ret = AAsset_getBuffer(m_asset);
#elif defined LOVE_ANDROID_JNI
		ret = m_buffer;
#endif

		if (!isOpen)
			close();

		return ret;
	}

	int64 File::getSizeOpened()
	{
#ifdef LOVE_WINDOWS
		struct stat filestatus;
		if (stat(filename.c_str(), &filestatus) >= 0) 
		{
			return filestatus.st_size;
		} 
		else 
		{
			return -1;
		}
#elif defined LOVE_ANDROID_NATIVE
		return AAsset_getLength(m_asset);
#elif defined LOVE_ANDROID_JNI
		return m_nSize;
#endif
	}

} // null
} // filesystem
} // love
