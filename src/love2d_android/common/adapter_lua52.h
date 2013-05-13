//#ifndef LOVE_ADAPTER_LUA52_H
//#define LOVE_ADAPTER_LUA52_H
//
//extern "C" {
//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>
//}
//
//#if (LUA_VERSION_NUM > 501)
//
//#define LUA_GLOBALSINDEX    LUA_RIDX_GLOBALS
//
//#define luaL_reg					luaL_Reg
////#define luaL_putchar(B,c)			luaL_addchar(B,c)
//#define lua_open					luaL_newstate
//#define lua_strlen					lua_rawlen
//#define lua_objlen					lua_rawlen
//
//inline LUALIB_API void luaL_putchar (luaL_Buffer *B, unsigned char c)
//{
//	luaL_addchar(B,c);
//}
//
//inline LUALIB_API void (luaL_register) (lua_State *L, const char *libname, const luaL_Reg *l) 
//{
//	lua_newtable(L);
//	luaL_setfuncs(L, l, 0);
//}
//
//inline LUALIB_API int luaL_typerror (lua_State *L, int narg, const char *tname) 
//{
//	const char *msg = lua_pushfstring(L, "%s expected, got %s",
//		tname, luaL_typename(L, narg));
//	return luaL_argerror(L, narg, msg);
//}
//
//#endif // #if (LUA_VERSION_NUM > 501)
//
//#endif // LOVE_ADAPTER_LUA52_H