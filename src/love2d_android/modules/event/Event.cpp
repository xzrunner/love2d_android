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

#include "Event.h"

using love::thread::Mutex;
using love::thread::Lock;

namespace love
{
namespace event
{
	Message::Message(std::string name, Variant *a, Variant *b, Variant *c, Variant *d)
		: name(name), nargs(0)
	{
		args[0] = a;
		args[1] = b;
		args[2] = c;
		args[3] = d;
		for (int i = 0; i < 4; i++)
		{
			if (!args[i])
				continue;
			args[i]->retain();
			nargs++;
		}
	}

	Message::~Message()
	{
		for (int i = 0; i < nargs; i++)
			args[i]->release();
	}

	int Message::toLua(lua_State *L)
	{
		luax_pushstring(L, name);
		for (int i = 0; i < nargs; i++)
			args[i]->toLua(L);
		return nargs+1;
	}

	Message *Message::fromLua(lua_State *L, int n)
	{
		std::string name = luax_checkstring(L, n);
		n++;
		Message *m = new Message(name);
		for (int i = 0; i < 4; i++)
		{
			if (lua_isnoneornil(L, n+i))
				break;
			m->args[i] = Variant::fromLua(L, n+i);
			if (!m->args[i])
			{
				delete m;
				luaL_error(L, "Argument %d can't be stored safely\nExpected boolean, number, string or userdata.", n+i);
				return NULL;
			}
			m->nargs++;
		}
		return m;
	}

	Event::~Event()
	{
	}

	void Event::push(Message *msg)
	{
		Lock lock(mutex);
		msg->retain();
		queue.push(msg);
	}

	bool Event::poll(Message *&msg)
	{
		Lock lock(mutex);
		if (queue.empty())
			return false;
		msg = queue.front();
		queue.pop();
		return true;
	}

	void Event::clear()
	{
		Lock lock(mutex);
		while (!queue.empty())
		{
			queue.back()->release();
			queue.pop();
		}
	}

	bool Event::getConstant(const char * in, love::mouse::Mouse::Button & out)
	{
		return buttons.find(in, out);
	}

	bool Event::getConstant(love::mouse::Mouse::Button in, const char *& out)
	{
		return buttons.find(in, out);
	}

	bool Event::getConstant(const char * in, love::keyboard::Keyboard::Key & out)
	{
		return keys.find(in, out);
	}

	bool Event::getConstant(love::keyboard::Keyboard::Key in, const char *& out)
	{
		return keys.find(in, out);
	}

	StringMap<love::mouse::Mouse::Button, love::mouse::Mouse::BUTTON_MAX_ENUM>::Entry Event::buttonEntries[] =
	{
		{"l", love::mouse::Mouse::BUTTON_LEFT},
		{"m", love::mouse::Mouse::BUTTON_MIDDLE},
		{"r", love::mouse::Mouse::BUTTON_RIGHT},
		{"wu", love::mouse::Mouse::BUTTON_WHEELUP},
		{"wd", love::mouse::Mouse::BUTTON_WHEELDOWN},
		{"x1", love::mouse::Mouse::BUTTON_X1},
		{"x2", love::mouse::Mouse::BUTTON_X2},
	};

	StringMap<love::mouse::Mouse::Button, love::mouse::Mouse::BUTTON_MAX_ENUM> Event::buttons(Event::buttonEntries, sizeof(Event::buttonEntries));

	StringMap<love::keyboard::Keyboard::Key, love::keyboard::Keyboard::KEY_MAX_ENUM>::Entry Event::keyEntries[] =
	{
		{"escape", love::keyboard::Keyboard::KEY_ESCAPE},

		{"a", love::keyboard::Keyboard::KEY_A},
		{"b", love::keyboard::Keyboard::KEY_B},
		{"c", love::keyboard::Keyboard::KEY_C},
		{"d", love::keyboard::Keyboard::KEY_D},
		{"e", love::keyboard::Keyboard::KEY_E},
		{"f", love::keyboard::Keyboard::KEY_F},
		{"g", love::keyboard::Keyboard::KEY_G},
		{"h", love::keyboard::Keyboard::KEY_H},
		{"i", love::keyboard::Keyboard::KEY_I},
		{"j", love::keyboard::Keyboard::KEY_J},
		{"k", love::keyboard::Keyboard::KEY_K},
		{"l", love::keyboard::Keyboard::KEY_L},
		{"m", love::keyboard::Keyboard::KEY_M},
		{"n", love::keyboard::Keyboard::KEY_N},
		{"o", love::keyboard::Keyboard::KEY_O},
		{"p", love::keyboard::Keyboard::KEY_P},
		{"q", love::keyboard::Keyboard::KEY_Q},
		{"r", love::keyboard::Keyboard::KEY_R},
		{"s", love::keyboard::Keyboard::KEY_S},
		{"t", love::keyboard::Keyboard::KEY_T},
		{"u", love::keyboard::Keyboard::KEY_U},
		{"v", love::keyboard::Keyboard::KEY_V},
		{"w", love::keyboard::Keyboard::KEY_W},
		{"x", love::keyboard::Keyboard::KEY_X},
		{"y", love::keyboard::Keyboard::KEY_Y},
		{"z", love::keyboard::Keyboard::KEY_Z},

		{"unknown", love::keyboard::Keyboard::KEY_UNKNOWN},
	};

	StringMap<love::keyboard::Keyboard::Key, love::keyboard::Keyboard::KEY_MAX_ENUM> Event::keys(Event::keyEntries, sizeof(Event::keyEntries));

} // event
} // love
