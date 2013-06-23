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

#include <event/eventdefs.h>

extern float g_scale;
extern float g_xOffset;
extern float g_yOffset;

namespace love
{
namespace event
{
namespace null
{
	const char * Event::getName() const
	{
		return "love.event.null";
	}

	void Event::push_params(int type, int p1, int p2, int p3)
	{
		Message* msg = NULL;
		love::keyboard::Keyboard::Key key;
		love::mouse::Mouse::Button button;
		Variant *arg1, *arg2, *arg3;
		const char *txt;
		switch (type)
		{
		case LOVE_KEYDOWN:
			if (keys.find(keyboard::LKey(p1), key) && love::event::Event::keys.find(key, txt))
			{
				arg1 = new Variant(txt, strlen(txt));
				arg2 = new Variant((double) p1);
				msg = new Message("keypressed", arg1, arg2);
				arg1->release();
				arg2->release();
			}
			break;
		case LOVE_KEYUP:
			if (keys.find(keyboard::LKey(p1), key) && love::event::Event::keys.find(key, txt))
			{
				arg1 = new Variant(txt, strlen(txt));
				arg2 = new Variant((double) p1);
				msg = new Message("keyreleased", arg1, arg2);
				arg1->release();
				arg2->release();
			}
			break;
		case LOVE_MOUSEMOTION:
		case LOVE_MOUSEBUTTONDOWN:
		case LOVE_MOUSEBUTTONUP:
			if (buttons.find(mouse::LButton(p3), button) && love::event::Event::buttons.find(button, txt))
			{
				arg1 = new Variant((double) (p1 - g_xOffset) / g_scale);
				arg2 = new Variant((double) (p2 - g_yOffset) / g_scale);
				arg3 = new Variant(txt, strlen(txt));
				if (type == LOVE_MOUSEMOTION)
					msg = new Message("mousemoved", arg1, arg2, arg3);
				else if (type == LOVE_MOUSEBUTTONDOWN)
					msg = new Message("mousepressed", arg1, arg2, arg3);
				else
					msg = new Message("mousereleased", arg1, arg2, arg3);
				arg1->release();
				arg2->release();
				arg3->release();
			}
			break;
// 		default:
// 			msg = new Message("quit");
// 			break;
		}

		if (msg)
		{
			push(msg);
			msg->release();
		}
	}

	EnumMap<love::keyboard::Keyboard::Key, keyboard::LKey, love::keyboard::Keyboard::KEY_MAX_ENUM>::Entry Event::keyEntries[] =
	{
		{ love::keyboard::Keyboard::KEY_ESCAPE, keyboard::LK_ESCAPE },

		{ love::keyboard::Keyboard::KEY_A, keyboard::LK_a },
		{ love::keyboard::Keyboard::KEY_B, keyboard::LK_b },
		{ love::keyboard::Keyboard::KEY_C, keyboard::LK_c },
		{ love::keyboard::Keyboard::KEY_D, keyboard::LK_d },
		{ love::keyboard::Keyboard::KEY_E, keyboard::LK_e },
		{ love::keyboard::Keyboard::KEY_F, keyboard::LK_f },
		{ love::keyboard::Keyboard::KEY_G, keyboard::LK_g },
		{ love::keyboard::Keyboard::KEY_H, keyboard::LK_h },
		{ love::keyboard::Keyboard::KEY_I, keyboard::LK_i },
		{ love::keyboard::Keyboard::KEY_J, keyboard::LK_j },
		{ love::keyboard::Keyboard::KEY_K, keyboard::LK_k },
		{ love::keyboard::Keyboard::KEY_L, keyboard::LK_l },
		{ love::keyboard::Keyboard::KEY_M, keyboard::LK_m },
		{ love::keyboard::Keyboard::KEY_N, keyboard::LK_n },
		{ love::keyboard::Keyboard::KEY_O, keyboard::LK_o },
		{ love::keyboard::Keyboard::KEY_P, keyboard::LK_p },
		{ love::keyboard::Keyboard::KEY_Q, keyboard::LK_q },
		{ love::keyboard::Keyboard::KEY_R, keyboard::LK_r },
		{ love::keyboard::Keyboard::KEY_S, keyboard::LK_s },
		{ love::keyboard::Keyboard::KEY_T, keyboard::LK_t },
		{ love::keyboard::Keyboard::KEY_U, keyboard::LK_u },
		{ love::keyboard::Keyboard::KEY_V, keyboard::LK_v },
		{ love::keyboard::Keyboard::KEY_W, keyboard::LK_w },
		{ love::keyboard::Keyboard::KEY_X, keyboard::LK_x },
		{ love::keyboard::Keyboard::KEY_Y, keyboard::LK_y },
		{ love::keyboard::Keyboard::KEY_Z, keyboard::LK_z },

		{ love::keyboard::Keyboard::KEY_UNKNOWN, keyboard::LK_UNKNOWN },
	};

	EnumMap<love::keyboard::Keyboard::Key, keyboard::LKey, love::keyboard::Keyboard::KEY_MAX_ENUM> Event::keys(Event::keyEntries, sizeof(Event::keyEntries));

	EnumMap<love::mouse::Mouse::Button, mouse::LButton, love::mouse::Mouse::BUTTON_MAX_ENUM>::Entry Event::buttonEntries[] =
	{
		{ love::mouse::Mouse::BUTTON_LEFT, mouse::LB_LEFT},
		{ love::mouse::Mouse::BUTTON_MIDDLE, mouse::LB_MIDDLE},
		{ love::mouse::Mouse::BUTTON_RIGHT, mouse::LB_RIGHT},
		{ love::mouse::Mouse::BUTTON_WHEELUP, mouse::LB_WHEELUP},
		{ love::mouse::Mouse::BUTTON_WHEELDOWN, mouse::LB_WHEELDOWN},
		{ love::mouse::Mouse::BUTTON_X1, mouse::LB_X1},
		{ love::mouse::Mouse::BUTTON_X2, mouse::LB_X2},
	};

	EnumMap<love::mouse::Mouse::Button, mouse::LButton, love::mouse::Mouse::BUTTON_MAX_ENUM> Event::buttons(Event::buttonEntries, sizeof(Event::buttonEntries));

} // null
} // event
} // love
