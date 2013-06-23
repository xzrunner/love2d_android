/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "Filesystem.h"

// LOVE
#include <common/b64.h>
#include <filesystem/FileData.h>

#ifdef LOVE_ANDROID_JNI
#include <Minizip/unzip.h>
#endif // LOVE_ANDROID_JNI

#include "File.h"

namespace love
{
namespace filesystem
{
namespace null
{

	Filesystem* Filesystem::m_instance = NULL;

	Filesystem* Filesystem::Instance()
	{
		if (!m_instance)
		{
			m_instance = new Filesystem;
		}
		return m_instance;
	}

	const char * Filesystem::getName() const
	{
		return "love.filesystem.null";
	}

	std::string Filesystem::getAbsolutePath(const std::string& filename) const
	{
#ifdef LOVE_WINDOWS
		return game_source + filename;
#else
		return filename;
#endif
	}

	bool Filesystem::setSource(const char * source)
	{
		// Save the game source.
		game_source = std::string(source);

		return true;
	}

	File * Filesystem::newFile(const char *filename)
	{
		return new File(getAbsolutePath(filename));
	}

	FileData * Filesystem::newFileData(void * data, unsigned int size, const char * filename)
	{
		FileData * fd = new FileData(size, std::string(filename));

		// Copy the data into FileData.
		memcpy(fd->getData(), data, size);

		return fd;
	}

	FileData * Filesystem::newFileData(const char * b64, const char * filename)
	{
		int size = strlen(b64);
		int outsize = 0;
		char * dst = b64_decode(b64, size, outsize);
		FileData * fd = new FileData(outsize, std::string(filename));

		// Copy the data into FileData.
		memcpy(fd->getData(), dst, outsize);
		delete [] dst;

		return fd;
	}

	bool Filesystem::exists(const char * file)
	{
#ifdef LOVE_WINDOWS
		FILE *f;
		if (f = fopen(getAbsolutePath(file).c_str(), "rb"))
		{
			fclose(f);
			return true;
		}
		return false;

#elif defined LOVE_ANDROID_JNI
		return isExists(file);
#endif
	}

	int Filesystem::load(lua_State * L)
	{
		// Need only one arg.
		luax_assert_argc(L, 1, 1);

		// Must be string.
		if (!lua_isstring(L, -1))
			return luaL_error(L, "The argument must be a string.");

		std::string filename = lua_tostring(L, -1);

		// The file must exist.
		if (!exists(filename.c_str()))
			return luaL_error(L, "File %s does not exist.", filename.c_str());

		// Create the file.
		File * file = newFile(filename.c_str());
		file->open(File::READ);

		// Get the data from the file.

		int status = luaL_loadbuffer(L, (const char *)file->bufferize(), file->getSize(), ("@" + filename).c_str());

		file->close();
		file->release();

		// Load the chunk, but don't run it.
		switch (status)
		{
		case LUA_ERRMEM:
			return luaL_error(L, "Memory allocation error: %s\n", lua_tostring(L, -1));
		case LUA_ERRSYNTAX:
			return luaL_error(L, "Syntax error: %s\n", lua_tostring(L, -1));
		default: // success
			return 1;
		}
	}

#ifdef LOVE_ANDROID_JNI

	void Filesystem::setResourcePath(const char* pszResourcePath)
	{
		std::string tmp(pszResourcePath);

		if ((! pszResourcePath) || tmp.find(".apk") == std::string::npos)
		{
			return;
		}

		game_source = pszResourcePath;
	}

	unsigned char* Filesystem::getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize)
	{    
		unsigned char* pData = 0;
		std::string fullPath(pszFileName);

		if ((! pszFileName) || (! pszMode))
		{
			return 0;
		}

		if (pszFileName[0] != '/')
		{
			// read from apk
			fullPath.insert(0, "assets/");
			pData =  getFileDataFromZip(game_source.c_str(), fullPath.c_str(), pSize);
		}
		else
		{
			do 
			{
				// read rrom other path than user set it
				FILE *fp = fopen(pszFileName, pszMode);
				if (!fp) break;

				unsigned long size;
				fseek(fp,0,SEEK_END);
				size = ftell(fp);
				fseek(fp,0,SEEK_SET);
				pData = new unsigned char[size];
				size = fread(pData,sizeof(unsigned char), size,fp);
				fclose(fp);

				if (pSize)
				{
					*pSize = size;
				}            
			} while (0);        
		}

		return pData;
	}

	unsigned char* Filesystem::getFileDataFromZip(const char* pszZipFilePath, const char* pszFileName, unsigned long * pSize)
	{
		unsigned char * pBuffer = NULL;
		unzFile pFile = NULL;
		*pSize = 0;

		do 
		{
			if (!pszZipFilePath || !pszFileName) break;
			if (strlen(pszZipFilePath) == 0) break;

			pFile = unzOpen(pszZipFilePath);
			if (!pFile) break;

			int nRet = unzLocateFile(pFile, pszFileName, 1);
			if (UNZ_OK != nRet) break;

			char szFilePathA[260];
			unz_file_info FileInfo;
			nRet = unzGetCurrentFileInfo(pFile, &FileInfo, szFilePathA, sizeof(szFilePathA), NULL, 0, NULL, 0);
			if (UNZ_OK != nRet) break;

			nRet = unzOpenCurrentFile(pFile);
			if (UNZ_OK != nRet) break;

			pBuffer = new unsigned char[FileInfo.uncompressed_size];
			int nSize = 0;
			nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);

			*pSize = FileInfo.uncompressed_size;

			unzCloseCurrentFile(pFile);
		} while (0);

		if (pFile)
		{
			unzClose(pFile);
		}

		return pBuffer;
	}

	bool Filesystem::isExists(const char* file)
	{
		unsigned char* pData = 0;
		std::string fullPath(file);

		if (!file) return false;

		if (file[0] != '/')
		{
			// read from apk
			fullPath.insert(0, "assets/");

			if (game_source.empty()) return false;
			unzFile pFile = unzOpen(game_source.c_str());
			if (!pFile) return false;
			int nRet = unzLocateFile(pFile, fullPath.c_str(), 1);
			if (UNZ_OK != nRet) return false;
			unzClose(pFile);
			return true;
		}
		else
		{
			// read rrom other path than user set it
			FILE *fp = fopen(file, "rb");
			if (fp)
			{
				fclose(fp);
				return true;
			}
			else
				return false; 
		}

		return pData;
	}

#endif // LOVE_ANDROID_JNI

} // null
} // filesystem
} // love