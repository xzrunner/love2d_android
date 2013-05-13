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

// LOVE
#include <common/log.h>
#include "wrap_Filesystem.h"

namespace love
{
namespace filesystem
{
namespace null
{
	static Filesystem * instance = 0;

// 	int w_getAbsolutePath(lua_State* L)
// 	{
// 		const char* filename = luaL_checkstring(L, 1);
// 		lua_pushstring(L, instance->getAbsolutePath(filename).c_str());
// 		return 1;
// 	}

	int w_setSource(lua_State * L)
	{
		const char * arg = luaL_checkstring(L, 1);

		if (!instance->setSource(arg))
			return luaL_error(L, "Could not set source.");

		return 0;
	}

	int w_newFile(lua_State * L)
	{
		const char * filename = luaL_checkstring(L, 1);
		File * t;
		try
		{
			t = instance->newFile(filename);
		}
		catch (Exception e)
		{
			return luaL_error(L, e.what());
		}
		luax_newtype(L, "File", FILESYSTEM_FILE_T, (void*)t);
		return 1;
	}

	int w_newFileData(lua_State * L)
	{
		if (!lua_isstring(L, 1))
			return luaL_error(L, "String expected.");
		if (!lua_isstring(L, 2))
			return luaL_error(L, "String expected.");

		size_t length = 0;
		const char * str = lua_tolstring(L, 1, &length);
		const char * filename = lua_tostring(L, 2);
		const char * decstr = lua_isstring(L, 3) ? lua_tostring(L, 3) : 0;

		FileData::Decoder decoder = FileData::FILE;

		if (decstr)
			FileData::getConstant(decstr, decoder);

		FileData * t = 0;

		switch(decoder)
		{
		case FileData::FILE:
			t = instance->newFileData((void*)str, (int)length, filename);
			break;
		case FileData::BASE64:
			t = instance->newFileData(str, filename);
			break;
		default:
			return luaL_error(L, "Unrecognized FileData decoder: %s", decstr);
		}

		luax_newtype(L, "FileData", FILESYSTEM_FILE_DATA_T, (void*)t);
		return 1;
	}

	int w_exists(lua_State * L)
	{
		const char * arg = luaL_checkstring(L, 1);
		lua_pushboolean(L, instance->exists(arg) ? 1 : 0);
		return 1;
	}

	int w_log(lua_State * L)
	{
		const char * arg = luaL_checkstring(L, 1);
		log(arg);
		return 0;
	}

#ifdef LOVE_ANDROID_JNI
	int w_setResourcePath(lua_State* L)
	{
		instance->setResourcePath(luaL_checkstring(L, 1));
		return 0;
	}

	int w_getFileData(lua_State* L)
	{
		return 0;
	}

	int w_getFileDataFromZip(lua_State* L)
	{
		return 0;
	}
#endif

	int loader(lua_State * L)
	{
		const char * filename = lua_tostring(L, -1);

		std::string tmp(filename);
		tmp += ".lua";

		int size = tmp.size();

		for (int i=0;i<size-4;i++)
		{
			if (tmp[i] == '.')
			{
				tmp[i] = '/';
			}
		}

		// Check whether file exists.
		if (instance->exists(tmp.c_str()))
		{
			lua_pop(L, 1);
			lua_pushstring(L, tmp.c_str());
			// Ok, load it.
			return instance->load(L);
		}

		tmp = filename;
		size = tmp.size();
		for (int i=0;i<size;i++)
		{
			if (tmp[i] == '.')
			{
				tmp[i] = '/';
			}
		}

		//if (instance->isDirectory(tmp.c_str()))
		//{
		//	tmp += "/init.lua";
		//	if (instance->exists(tmp.c_str()))
		//	{
		//		lua_pop(L, 1);
		//		lua_pushstring(L, tmp.c_str());
		//		// Ok, load it.
		//		return instance->load(L);
		//	}
		//}

		lua_pushfstring(L, "\n\tno file \"%s\" in LOVE game directories.\n", (tmp + ".lua").c_str());
		return 1;
	}

	// List of functions to wrap.
	static const luaL_Reg functions[] = {
//		{ "getAbsolutePath",  w_getAbsolutePath },
		{ "setSource",  w_setSource },
		{ "newFile",  w_newFile },
		{ "newFileData", w_newFileData },
		{ "exists",  w_exists },
		{ "log", w_log },
		{ 0, 0 }
	};

	static const lua_CFunction types[] = {
		luaopen_file,
		luaopen_filedata,
		0
	};

	extern "C" int luaopen_love_filesystem(lua_State * L)
	{
		if (instance == 0)
		{
			try
			{
				instance = Filesystem::Instance();
 				love::luax_register_searcher(L, loader, 1);
 				//love::luax_register_searcher(L, extloader, 2);
			}
			catch (Exception & e)
			{
				return luaL_error(L, e.what());
			}
		}
		else
		{
			// singleton, no new, so...
			//instance->retain();
			
 			love::luax_register_searcher(L, loader, 1);
 			//love::luax_register_searcher(L, extloader, 2);
		}

		WrappedModule w;
		w.module = instance;
		w.name = "filesystem";
		w.flags = MODULE_FILESYSTEM_T;
		w.functions = functions;
		w.types = types;

		return luax_register_module(L, w);
	}

} // null
} // filesystem
} // love
