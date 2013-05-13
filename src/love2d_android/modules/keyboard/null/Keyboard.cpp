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
namespace null
{
	Keyboard::Keyboard()
	{
		memset(keystate, 0, sizeof(bool) * KEY_MAX_ENUM);
	}

	const char * Keyboard::getName() const
	{
		return "love.keyboard.null";
	}

	bool Keyboard::isDown(Key * keylist) const
	{
		for (Key key = *keylist; key != KEY_MAX_ENUM; key = *(++keylist))
		{
			if (keystate[(unsigned)key] == true)
				return true;
		}

		return false;
	}

	void Keyboard::setKeyState(Key key, bool isDown)
	{
		if (key < KEY_MAX_ENUM)
			keystate[key] = isDown;
	}

	EnumMap<Keyboard::Key, LKey, Keyboard::KEY_MAX_ENUM>::Entry Keyboard::keyEntries[] =
	{
		{ Keyboard::KEY_ESCAPE, LK_ESCAPE },

		{ Keyboard::KEY_A, LK_a },
		{ Keyboard::KEY_B, LK_b },
		{ Keyboard::KEY_C, LK_c },
		{ Keyboard::KEY_D, LK_d },
		{ Keyboard::KEY_E, LK_e },
		{ Keyboard::KEY_F, LK_f },
		{ Keyboard::KEY_G, LK_g },
		{ Keyboard::KEY_H, LK_h },
		{ Keyboard::KEY_I, LK_i },
		{ Keyboard::KEY_J, LK_j },
		{ Keyboard::KEY_K, LK_k },
		{ Keyboard::KEY_L, LK_l },
		{ Keyboard::KEY_M, LK_m },
		{ Keyboard::KEY_N, LK_n },
		{ Keyboard::KEY_O, LK_o },
		{ Keyboard::KEY_P, LK_p },
		{ Keyboard::KEY_Q, LK_q },
		{ Keyboard::KEY_R, LK_r },
		{ Keyboard::KEY_S, LK_s },
		{ Keyboard::KEY_T, LK_t },
		{ Keyboard::KEY_U, LK_u },
		{ Keyboard::KEY_V, LK_v },
		{ Keyboard::KEY_W, LK_w },
		{ Keyboard::KEY_X, LK_x },
		{ Keyboard::KEY_Y, LK_y },
		{ Keyboard::KEY_Z, LK_z },
	};

	EnumMap<Keyboard::Key, LKey, Keyboard::KEY_MAX_ENUM> Keyboard::keys(Keyboard::keyEntries, sizeof(Keyboard::keyEntries));


} // null
} // keyboard
} // love
