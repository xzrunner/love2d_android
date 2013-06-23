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

#ifndef LOVE_KEYBOARD_KEYBOARD_H
#define LOVE_KEYBOARD_KEYBOARD_H

// LOVE
#include <common/Module.h>
#include <common/StringMap.h>

namespace love
{
namespace keyboard
{
	class Keyboard : public Module
	{
	public:

		enum Key
		{
			KEY_UNKNOWN,
			KEY_ESCAPE,

			KEY_A,
			KEY_B,
			KEY_C,
			KEY_D,
			KEY_E,
			KEY_F,
			KEY_G,
			KEY_H,
			KEY_I,
			KEY_J,
			KEY_K,
			KEY_L,
			KEY_M,
			KEY_N,
			KEY_O,
			KEY_P,
			KEY_Q,
			KEY_R,
			KEY_S,
			KEY_T,
			KEY_U,
			KEY_V,
			KEY_W,
			KEY_X,
			KEY_Y,
			KEY_Z,

			KEY_MAX_ENUM = 512
		};

		static const int DEFAULT = -1;

		virtual ~Keyboard(){}

		/**
		* Checks whether a certain key is down or not.
		* @param key A key identifier.
		* @return boolean
		**/
		virtual bool isDown(Key * keylist) const = 0;

		/**
		* Set key is down or not.
		* @param key A key identifier
		* @param isDown Key is down state
		**/
		virtual void setKeyState(Key key, bool isDown) = 0;

		static bool getConstant(const char * in, Key & out);
		static bool getConstant(Key in, const char *& out);

	private:

		static StringMap<Key, KEY_MAX_ENUM>::Entry keyEntries[];
		static StringMap<Key, KEY_MAX_ENUM> keys;

	}; // Keyboard

} // keyboard
} // love

#endif // LOVE_KEYBOARD_KEYBOARD_H
