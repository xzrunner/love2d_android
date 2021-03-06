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

#include <common/config.h>

#include "wrap_Keyboard.h"

#include "null/Keyboard.h"

namespace love
{
namespace keyboard
{
	static Keyboard * instance;

	int w_isDown(lua_State * L)
	{
		Keyboard::Key k;
		unsigned int num = lua_gettop(L);
		Keyboard::Key * keylist = new Keyboard::Key[num+1];
		unsigned int counter = 0;

		for (unsigned int i = 0; i < num; i++)
		{
			if (Keyboard::getConstant(luaL_checkstring(L, i+1), k))
				keylist[counter++] = k;
		}
		keylist[counter] = Keyboard::KEY_MAX_ENUM;

		luax_pushboolean(L, instance->isDown(keylist));
		delete[] keylist;
		return 1;
	}

	int w_setKeyState(lua_State * L)
	{
		Keyboard::Key k;
		if (Keyboard::getConstant(luaL_checkstring(L, 1), k))
		{
			bool isDown = (bool)luax_toboolean(L, 2);
			instance->setKeyState(k, isDown);
		}
		return 0;
	}

	// List of functions to wrap.
	static const luaL_Reg functions[] = {
		{ "isDown", w_isDown },
		{ "setKeyState", w_setKeyState },
		{ 0, 0 }
	};

	extern "C" int luaopen_love_keyboard(lua_State * L)
	{
		if (instance == 0)
		{
			try
			{
				instance = new love::keyboard::null::Keyboard();
			}
			catch (Exception & e)
			{
				return luaL_error(L, e.what());
			}
		}
		else
			instance->retain();

		WrappedModule w;
		w.module = instance;
		w.name = "keyboard";
		w.flags = MODULE_T;
		w.functions = functions;
		w.types = 0;

		return luax_register_module(L, w);
	}
} // keyboard
} // love
