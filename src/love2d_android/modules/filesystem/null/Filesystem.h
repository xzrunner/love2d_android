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

#ifndef LOVE_FILESYSTEM_NULL_FILESYSTEM_H
#define LOVE_FILESYSTEM_NULL_FILESYSTEM_H

// LOVE
#include <common/Module.h>
#include <common/config.h>

namespace love
{
namespace filesystem
{
	class FileData;

namespace null
{
	class File;

	class Filesystem : public Module
	{
	public:

		static Filesystem* Instance();

		const char * getName() const;

		std::string getAbsolutePath(const std::string& filename) const;

		/**
		* Sets the path to the game source.
		* This can only be set once.
		* @param source Path to a directory or a .love-file.
		**/
		bool setSource(const char * source);

		/**
		* Creates a new file.
		**/
		File * newFile(const char* filename);

		/**
		* Creates a new FileData object. Data will be copied.
		* @param data Pointer to the data.
		* @param size The size of the data.
		* @param filename The full filename used to file type identification.
		**/
		FileData * newFileData(void * data, unsigned int size, const char * filename);

		/**
		* Creates a new FileData object from base64 data.
		* @param b64 The base64 data.
		**/
		FileData * newFileData(const char * b64, const char * filename);

		/**
		* Checks whether a file exists in the current search path
		* or not.
		* @param file The filename to check.
		**/
		bool exists(const char * file);

		/**
		* Loads a file without running it. The loaded
		* chunk is returned as a function.
		* @param filename The filename of the file to load.
		* @return A function.
		**/
		int load(lua_State * L);

#ifdef LOVE_ANDROID_JNI
		// from cocos2d-x
		void setResourcePath(const char *pszResourcePath);
		unsigned char* getFileData(const char* pszFileName, 
			const char* pszMode, unsigned long * pSize);
		unsigned char* getFileDataFromZip(const char* pszZipFilePath, 
			const char* pszFileName, unsigned long * pSize);
		bool isExists(const char * file);
#endif // LOVE_ANDROID_JNI

	protected:
		Filesystem() {}

	private:
		static Filesystem* m_instance;

		// The full path to the source of the game.
		std::string game_source;

	}; // Filesystem

} // null
} // filesystem
} // love

#endif // LOVE_FILESYSTEM_NULL_FILESYSTEM_H