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
#include <common/config.h>
#include <common/version.h>
#include <common/runtime.h>
#include <common/log.h>
#include <event/eventdefs.h>
#include <keyboard/keydefs.h>
#include <mouse/mousedefs.h>

#ifdef LOVE_WINDOWS
#	include <esUtil.h>
#else
#	include <GLES/gl.h>
#endif

#ifdef LOVE_BUILD_EXE

// Libraries.
#include "libraries/luasocket/luasocket.h"

// LOVE
#ifdef LOVE_ANDROID_JNI
#	include <modules/filesystem/null/Filesystem.h>
#endif // LOVE_ANDROID_JNI

#endif // LOVE_BUILD_EXE

#ifdef LOVE_BUILD_STANDALONE

// All modules define a c-accessible luaopen
// so let's make use of those, instead
// of addressing implementations directly.
extern "C"
{
	extern int luaopen_love_audio(lua_State*);
	extern int luaopen_love_event(lua_State*);
	extern int luaopen_love_filesystem(lua_State*);
	extern int luaopen_love_font(lua_State*);
	extern int luaopen_love_graphics(lua_State*);
	extern int luaopen_love_image(lua_State*);
	extern int luaopen_love_keyboard(lua_State*);
	extern int luaopen_love_mouse(lua_State*);
	extern int luaopen_love_physics(lua_State*);
	extern int luaopen_love_sound(lua_State*);
	extern int luaopen_love_timer(lua_State*);
	extern int luaopen_love_boot(lua_State*);
}

static const luaL_Reg modules[] = {
	{ "love.audio", luaopen_love_audio },
	{ "love.event", luaopen_love_event },
	{ "love.filesystem", luaopen_love_filesystem },
	{ "love.font", luaopen_love_font },
	{ "love.graphics", luaopen_love_graphics },
	{ "love.image", luaopen_love_image },
	{ "love.keyboard", luaopen_love_keyboard },
	{ "love.mouse", luaopen_love_mouse },
	{ "love.physics", luaopen_love_physics },
	{ "love.sound", luaopen_love_sound },
	{ "love.timer", luaopen_love_timer },
	{ "love.boot", luaopen_love_boot },
	{ 0, 0 }
};

#endif // LOVE_BUILD_STANDALONE

extern "C" LOVE_EXPORT int luaopen_love(lua_State * L)
{
	love::luax_insistglobal(L, "love");

	// Set version information.
	lua_pushstring(L, love::VERSION);
	lua_setfield(L, -2, "_version");

	lua_pushnumber(L, love::VERSION_MAJOR);
	lua_setfield(L, -2, "_version_major");
	lua_pushnumber(L, love::VERSION_MINOR);
	lua_setfield(L, -2, "_version_minor");
	lua_pushnumber(L, love::VERSION_REV);
	lua_setfield(L, -2, "_version_revision");

	lua_pushstring(L, love::VERSION_CODENAME);
	lua_setfield(L, -2, "_version_codename");

	lua_newtable(L);

	for (int i = 0; love::VERSION_COMPATIBILITY[i] != 0; ++i)
	{
		lua_pushstring(L, love::VERSION_COMPATIBILITY[i]);
		lua_rawseti(L, -2, i+1);
	}

	lua_setfield(L, -2, "_version_compat");

#ifdef LOVE_WINDOWS
	lua_pushstring(L, "Windows");
#elif defined(LOVE_MACOSX)
	lua_pushstring(L, "OS X");
#elif defined(LOVE_LINUX)
	lua_pushstring(L, "Linux");
#else
	lua_pushstring(L, "Unknown");
#endif
	lua_setfield(L, -2, "_os");

#ifdef LOVE_BUILD_STANDALONE

	// Preload module loaders.
	for (int i = 0; modules[i].name != 0; i++)
	{
		love::luax_preload(L, modules[i].func, modules[i].name);
	}

//	love::luasocket::__open(L);

#endif // LOVE_BUILD_STANDALONE

	return 1;
}

extern "C" int luaopen_love_boot(lua_State *L)
{
#ifdef LOVE_WINDOWS
	if (luaL_loadfile(L, "../../../src/love2d_android/scripts/boot.lua") == 0)
		lua_call(L, 0, 1);
#elif defined LOVE_ANDROID_JNI
	long unsigned int size;
	unsigned char* buffer = love::filesystem::null::Filesystem::Instance()->getFileData("boot.lua", "r", &size);
	if (luaL_loadbuffer(L, (const char*)buffer, size, "boot.lua") == 0)
		lua_call(L, 0, 1);
	delete[] buffer;
#endif
	return 1;
}

lua_State * L;

extern float g_width;
extern float g_height;
extern float g_scale;
extern float g_xOffset;
extern float g_yOffset;

void resize_common(int width, int height)
{
	g_width = width;
	g_height = height;

	const int WIDTH = 800,
		HEIGHT = 600;

	const float sw = (float)width / WIDTH,
		sh = (float)height / HEIGHT;
	g_scale = sw < sh ? sw : sh;

	glViewport(0, 0, width, height);

	// Reset the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set up orthographic view (no depth)
	glOrthof(0.0, width, height, 0.0, -1.0, 1.0);

	glScalef(sw, sh, 0.0f);

	// Reset modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//void resize_common(int width, int height)
//{
//	g_width = width;
//	g_height = height;
//
//	const int WIDTH = 800,
//		HEIGHT = 600;
//
//	const float sw = (float)width / WIDTH,
//		sh = (float)height / HEIGHT;
//	g_scale = sw < sh ? sw : sh;
//
//	// Set the viewport to top-left corner
//	if (g_scale == sw)
//	{
//		const float offset = (height - g_scale * HEIGHT) * 0.5f;
//		height -= offset * 2;
//		glViewport(0, offset, width, height);
//
//		g_xOffset = 0;
//		g_yOffset = offset;
//	}
//	else
//	{
//		const float offset = (width - g_scale * WIDTH) * 0.5f;
//		width -= offset * 2;
//		glViewport(offset, 0, width, height);
//
//		g_xOffset = offset;
//		g_yOffset = 0;
//	}
//
//	// Reset the projection matrix
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//
//	// Set up orthographic view (no depth)
//	glOrthof(0.0, width, height, 0.0, -1.0, 1.0);
//
//	glScalef(g_scale, g_scale, 0.0f);
//
//	// Reset modelview matrix
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}

#ifdef LOVE_WINDOWS

// from Graphics::setMode
void main_init(ESContext* esContext)
{
	// Enable blending
	glEnable(GL_BLEND);

	// "Normal" blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable line/point smoothing.
	//	setLineStyle(LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	// Enable textures
	glEnable(GL_TEXTURE_2D);
}

void main_update(ESContext* esContext, float deltaTime)
{
	if (L == 0) return;

	// call _love_update()
	lua_getfield(L, LUA_GLOBALSINDEX, "_love_update");
	int result = lua_pcall(L, 0, 0, 0);
	if (result != 0)
	{
		log(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void main_draw(ESContext* esContext)
{
	if (L == 0) return;

	static bool init = false;
	if (!init)
	{
		init = true;
		main_init(esContext);
	}

	// call _love_draw()
	lua_getfield(L, LUA_GLOBALSINDEX, "_love_draw");
	int result = lua_pcall(L, 0, 0, 0);
	if (result != 0)
	{
		log(lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void main_key(ESContext* esContext, int action, int keyCode, int unicode)
{
	if (L == 0) return;

	// call _love_key
	lua_getfield(L, LUA_GLOBALSINDEX, "_love_key");
	if (action == ES_KEY_DOWN)
		lua_pushinteger(L, love::event::LOVE_KEYDOWN);
	else if (action == ES_KEY_UP)
		lua_pushinteger(L, love::event::LOVE_KEYUP);
	else 
		return;

	if (keyCode == VK_ESCAPE)
		lua_pushinteger(L, love::keyboard::LK_ESCAPE);
	else if (keyCode >= 'A' && keyCode <= 'Z')
		lua_pushinteger(L, love::keyboard::LK_a + (keyCode - 'A'));
	else
		lua_pushinteger(L, love::keyboard::LK_UNKNOWN);

	int result = lua_pcall(L, 2, 0, 0);
	if (result != 0)
	{
		log(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void main_mouse(ESContext* esContext, int action, int x, int y, int buttons)
{
	if (L == 0) return;

	// call _love_mouse
	lua_getfield(L, LUA_GLOBALSINDEX, "_love_mouse");
	if (action == ES_MOUSE_MOVE)
		lua_pushinteger(L, love::event::LOVE_MOUSEMOTION);
	else if (action == ES_MOUSE_DOWN)
		lua_pushinteger(L, love::event::LOVE_MOUSEBUTTONDOWN);
	else if (action == ES_MOUSE_UP)
		lua_pushinteger(L, love::event::LOVE_MOUSEBUTTONUP);
	else if (action == ES_MOUSE_WHEEL)
		lua_pushinteger(L, love::event::LOVE_MOUSEBUTTONDOWN);
	else
		return;

	lua_pushinteger(L, x);
	lua_pushinteger(L, y);

	if (action == ES_MOUSE_WHEEL)
	{
		if (x > 0)
			lua_pushinteger(L, love::mouse::LB_WHEELUP);
		else
			lua_pushinteger(L, love::mouse::LB_WHEELDOWN);
	}
	else
	{
		switch (buttons)
		{
		case ES_MOUSE_LBUTTON:
			lua_pushinteger(L, love::mouse::LB_LEFT);
			break;
		case ES_MOUSE_MBUTTON:
			lua_pushinteger(L, love::mouse::LB_MIDDLE);
			break;
		case ES_MOUSE_RBUTTON:
			lua_pushinteger(L, love::mouse::LB_RIGHT);
			break;
		default:
			lua_pushinteger(L, love::mouse::LB_LEFT);
		}
	}

	int result = lua_pcall(L, 4, 0, 0);
	if (result != 0)
	{
		log(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void main_resize(ESContext* esContext, int width, int height)
{
	resize_common(width, height);
}

void initEGL()
{
	ESContext esContext;

	esInitContext(&esContext);
//	std::wstring title = L"test";
	std::string title = "test";
	esCreateWindow(&esContext, title.c_str(), 800, 600, ES_WINDOW_RGB);
	esRegisterDrawFunc(&esContext, main_draw);
	esRegisterUpdateFunc(&esContext, main_update);
	esRegisterKeyFunc(&esContext, main_key);
	esRegisterMouseFunc(&esContext, main_mouse);
	esRegisterResizeFunc(&esContext, main_resize);
	esMainLoop(&esContext);
}

#else

extern "C" void init(int argc, char ** argv)
{
	// Oh, you just want the version? Okay!
	if (argc > 1 && strcmp(argv[1],"--version") == 0)
	{
		printf("LOVE %s (%s)\n", love::VERSION, love::VERSION_CODENAME);
		return;
	}

	// Create the virtual machine.
	L = lua_open();
	luaL_openlibs(L);

	love::luax_preload(L, luaopen_love, "love");

	luaopen_love(L);

	// Add command line arguments to global arg (like stand-alone Lua).
	{
		lua_newtable(L);

		if (argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}

		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for (int i = 1; i<argc; i++)
		{
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
		}

		lua_setglobal(L, "arg");
	}

	// Add love.__exe = true.
	// This indicates that we're running the
	// standalone version of love, and not the
	// DLL version.
	{
		lua_getglobal(L, "love");
		lua_pushboolean(L, 1);
		lua_setfield(L, -2, "_exe");
		lua_pop(L, 1);
	}

	// Boot
	luaopen_love_boot(L);
	lua_call(L, 0, 1);
}

extern "C" void resize(int w, int h)
{
	// Enable blending
	glEnable(GL_BLEND);

	// "Normal" blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable line/point smoothing.
	//	setLineStyle(LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	// Enable textures
	glEnable(GL_TEXTURE_2D);

	resize_common(w, h);
}

// https://github.com/hagish/love-native-android
extern "C" void step()
{
	if (L == 0) return;

	{
		// call _love_update()
		lua_getfield(L, LUA_GLOBALSINDEX, "_love_update");
		int result = lua_pcall(L, 0, 0, 0);
		if (result != 0)
		{
			log(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	{
		// call _love_draw()
		lua_getfield(L, LUA_GLOBALSINDEX, "_love_draw");
		int result = lua_pcall(L, 0, 0, 0);
		if (result != 0)
		{
			log(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
}

extern "C" void touch(int type, int x, int y)
{
	if (L == 0) return;

	// call _love_mouse
	lua_getfield(L, LUA_GLOBALSINDEX, "_love_mouse");
	if (type == love::event::LOVE_MOUSEMOTION)
		lua_pushinteger(L, love::event::LOVE_MOUSEMOTION);
	else if (type == love::event::LOVE_MOUSEBUTTONDOWN)
		lua_pushinteger(L, love::event::LOVE_MOUSEBUTTONDOWN);
	else if (type == love::event::LOVE_MOUSEBUTTONUP)
		lua_pushinteger(L, love::event::LOVE_MOUSEBUTTONUP);
	else
		return;

	lua_pushinteger(L, x);
	lua_pushinteger(L, y);

	lua_pushinteger(L, love::mouse::LB_LEFT);

	int result = lua_pcall(L, 4, 0, 0);
	if (result != 0)
	{
		log(lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

#endif // LOVE_WINDOWS

extern "C" LOVE_EXPORT int loveandroidmain(int argc, char ** argv)
{
	// Oh, you just want the version? Okay!
	if (argc > 1 && strcmp(argv[1],"--version") == 0)
	{
		printf("LOVE %s (%s)\n", love::VERSION, love::VERSION_CODENAME);
		return 0;
	}

	// Create the virtual machine.
	L = lua_open();
	luaL_openlibs(L);

	love::luax_preload(L, luaopen_love, "love");

	luaopen_love(L);

	// Add command line arguments to global arg (like stand-alone Lua).
	{
		lua_newtable(L);

		if (argc > 0)
		{
			lua_pushstring(L, argv[0]);
			lua_rawseti(L, -2, -2);
		}

		lua_pushstring(L, "embedded boot.lua");
		lua_rawseti(L, -2, -1);

		for (int i = 1; i<argc; i++)
		{
			lua_pushstring(L, argv[i]);
			lua_rawseti(L, -2, i);
		}

		lua_setglobal(L, "arg");
	}

	// Add love.__exe = true.
	// This indicates that we're running the
	// standalone version of love, and not the
	// DLL version.
	{
		lua_getglobal(L, "love");
		lua_pushboolean(L, 1);
		lua_setfield(L, -2, "_exe");
		lua_pop(L, 1);
	}

//#ifdef LOVE_WINDOWS
//	initEGL();
//#endif // LOVE_WINDOWS

	// Boot
	luaopen_love_boot(L);
	lua_call(L, 0, 1);

#ifdef LOVE_WINDOWS
	initEGL();
#endif // LOVE_WINDOWS

	int retval = 0;
	if (lua_isnumber(L, 1))
		retval = (int) lua_tonumber(L, 1);

	lua_close(L);

	return retval;
}