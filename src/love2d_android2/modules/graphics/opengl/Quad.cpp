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

#include "Quad.h"

// for memcpy
#include <cstring>

// LOVE
#include <common/config.h>

namespace love
{
namespace graphics
{
namespace opengl
{
	Quad::Quad(const Viewport& v, float sw, float sh, AlignmentType alignment /*= CENTER*/)
		: alignment(alignment)
	{
		init(v, sw, sh);
	}

	Quad::Quad(float sw, float sh, AlignmentType alignment /*= CENTER*/)
		: alignment(alignment)
	{
		Viewport v;
		v.x = 0;
		v.y = 0;
		v.w = sw;
		v.h = sh;
		init(v, sw, sh);
	}

	Quad::Quad(float x, float y, float w, float h, float sw, float sh, AlignmentType alignment /*= CENTER*/)
		: alignment(alignment)
	{
		Viewport v;
		v.x = x;
		v.y = y;
		v.w = w;
		v.h = h;
		init(v, sw, sh);
	}

	void Quad::refresh(const Viewport& v, float sw, float sh)
	{
		if (!hasNpot())
		{
			sw = next_p2(sw);
			sh = next_p2(sh);
		}
		viewport = v;

		switch (alignment)
		{
		case LEFT_LOW:
			{
				vertices[0].x = 0;
				vertices[0].y = 0;
				vertices[1].x = 0;
				vertices[1].y = v.h;
				vertices[2].x = v.w;
				vertices[2].y = v.h;
				vertices[3].x = v.w;
				vertices[3].y = 0;
			}
			break;
		case CENTER:
		default:
			{
				const float hw = v.w * 0.5f,
					hh = v.h * 0.5f;

				vertices[0].x = -hw;
				vertices[0].y = -hh;
				vertices[1].x = -hw;
				vertices[1].y = hh;
				vertices[2].x = hw;
				vertices[2].y = hh;
				vertices[3].x = hw;
				vertices[3].y = -hh;
			}
			break;
		}

		vertices[0].s = v.x/sw;
		vertices[0].t = v.y/sh;
		vertices[1].s = v.x/sw;
		vertices[1].t = (v.y+v.h)/sh;
		vertices[2].s = (v.x+v.w)/sw;
		vertices[2].t = (v.y+v.h)/sh;
		vertices[3].s = (v.x+v.w)/sw;
		vertices[3].t = v.y/sh;
	}

	void Quad::setViewport(const Viewport& v)
	{
		refresh(v, sw, sh);
	}

	Quad::Viewport Quad::getViewport() const
	{
		return viewport;
	}

	void Quad::flip(bool x, bool y)
	{
		vertex temp[4];
		if (x)
		{
			memcpy(temp, vertices, sizeof(vertex)*NUM_VERTICES);
			vertices[0].s = temp[3].s; vertices[0].t = temp[3].t;
			vertices[1].s = temp[2].s; vertices[1].t = temp[2].t;
			vertices[2].s = temp[1].s; vertices[2].t = temp[1].t;
			vertices[3].s = temp[0].s; vertices[3].t = temp[0].t;
		}
		if (y)
		{
			memcpy(temp, vertices, sizeof(vertex)*NUM_VERTICES);
			vertices[0].s = temp[1].s; vertices[0].t = temp[1].t;
			vertices[1].s = temp[0].s; vertices[1].t = temp[0].t;
			vertices[2].s = temp[3].s; vertices[2].t = temp[3].t;
			vertices[3].s = temp[2].s; vertices[3].t = temp[2].t;
		}
	}

	void Quad::mirror(bool x, bool y)
	{
		for (size_t i = 0; i < NUM_VERTICES; ++i)
		{
			if (x)
				vertices[i].s = 1.0f - vertices[i].s;
			if (y)
				vertices[i].t = 1.0f - vertices[i].t;
		}
	}

	const vertex* Quad::getVertices() const
	{
		return vertices;
	}

	void Quad::scale(float scale)
	{
		for (size_t i = 0; i < NUM_VERTICES; ++i)
		{
			vertices[i].x *= scale;
			vertices[i].y *= scale;
		}
	}

	void Quad::init(const Viewport& v, float sw, float sh)
	{
		this->sw = sw;
		this->sh = sh;

		memset(vertices, 255, sizeof(vertex)*NUM_VERTICES);
		refresh(v, sw, sh);
	}

	bool Quad::hasNpot()
	{
#ifdef LOVE_WINDOWS
		return true;
#else
		return false;
#endif // LOVE_WINDOWS
	}

} // opengl
} // graphics
} // love