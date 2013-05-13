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

#include "Keyboard.h"

namespace love
{
namespace keyboard
{
	bool Keyboard::getConstant(const char * in, Keyboard::Key & out)
	{
		return keys.find(in, out);
	}

	bool Keyboard::getConstant(Keyboard::Key in, const char *& out)
	{
		return keys.find(in, out);
	}

	StringMap<Keyboard::Key, Keyboard::KEY_MAX_ENUM>::Entry Keyboard::keyEntries[] =
	{
		{"escape", Keyboard::KEY_ESCAPE},

		{"a", Keyboard::KEY_A},
		{"b", Keyboard::KEY_B},
		{"c", Keyboard::KEY_C},
		{"d", Keyboard::KEY_D},
		{"e", Keyboard::KEY_E},
		{"f", Keyboard::KEY_F},
		{"g", Keyboard::KEY_G},
		{"h", Keyboard::KEY_H},
		{"i", Keyboard::KEY_I},
		{"j", Keyboard::KEY_J},
		{"k", Keyboard::KEY_K},
		{"l", Keyboard::KEY_L},
		{"m", Keyboard::KEY_M},
		{"n", Keyboard::KEY_N},
		{"o", Keyboard::KEY_O},
		{"p", Keyboard::KEY_P},
		{"q", Keyboard::KEY_Q},
		{"r", Keyboard::KEY_R},
		{"s", Keyboard::KEY_S},
		{"t", Keyboard::KEY_T},
		{"u", Keyboard::KEY_U},
		{"v", Keyboard::KEY_V},
		{"w", Keyboard::KEY_W},
		{"x", Keyboard::KEY_X},
		{"y", Keyboard::KEY_Y},
		{"z", Keyboard::KEY_Z},
	};

	StringMap<Keyboard::Key, Keyboard::KEY_MAX_ENUM> Keyboard::keys(Keyboard::keyEntries, sizeof(Keyboard::keyEntries));

} // keyboard
} // love
