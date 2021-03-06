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

#ifndef LOVE_EVENT_NULL_EVENT_H
#define LOVE_EVENT_NULL_EVENT_H

// LOVE
#include <event/Event.h>
#include <keyboard/keydefs.h>
#include <mouse/mousedefs.h>
#include <common/runtime.h>
#include <common/EnumMap.h>

namespace love
{
namespace event
{
namespace null
{
	class Event : public love::event::Event
	{
	public:

		// Implements Module.
		const char * getName() const;		

		void push_params(int type, int p1, int p2, int p3);

	private:

		static EnumMap<love::keyboard::Keyboard::Key, keyboard::LKey, love::keyboard::Keyboard::KEY_MAX_ENUM>::Entry keyEntries[];
		static EnumMap<love::keyboard::Keyboard::Key, keyboard::LKey, love::keyboard::Keyboard::KEY_MAX_ENUM> keys;
		static EnumMap<love::mouse::Mouse::Button, mouse::LButton, love::mouse::Mouse::BUTTON_MAX_ENUM>::Entry buttonEntries[];
		static EnumMap<love::mouse::Mouse::Button, mouse::LButton, love::mouse::Mouse::BUTTON_MAX_ENUM> buttons;

	}; // System

} // null
} // event
} // love

#endif // LOVE_EVENT_NULL_EVENT_H
