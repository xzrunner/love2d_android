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

#include "Mouse.h"

namespace love
{
namespace mouse
{
namespace null
{
	Mouse::Mouse()
	{
		x = y = 0;
		memset(buttonstate, 0, sizeof(bool) * BUTTON_MAX_ENUM);
	}

	const char * Mouse::getName() const
	{
		return "love.mouse.null";
	}

	int Mouse::getX() const
	{
		return x;
	}

	int Mouse::getY() const
	{
		return y;
	}

	void Mouse::getPosition(int & x, int & y) const
	{
		x = this->x;
		y = this->y;
	}

	void Mouse::setPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void Mouse::setX(int x)
	{
		this->x = x;
	}

	void Mouse::setY(int y)
	{
		this->y = y;
	}

	bool Mouse::isDown(Button * buttonlist) const
	{
		for (Button button = *buttonlist; button != BUTTON_MAX_ENUM; button = *(++buttonlist))
		{
			if (button < BUTTON_MAX_ENUM && buttonstate[button])
				return true;
		}

		return false;
	}

	void Mouse::setButtonState(Button btn, bool isDown)
	{
		if (btn < BUTTON_MAX_ENUM)
			buttonstate[btn] = isDown;
	}

} // null
} // mouse
} // love
