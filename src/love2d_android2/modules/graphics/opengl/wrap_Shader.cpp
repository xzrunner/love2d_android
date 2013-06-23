//#include "wrap_Shader.h"
//
//namespace love
//{
//namespace graphics
//{
//namespace opengl
//{
//
//	Shader* luax_checkshader(lua_State * L, int idx)
//	{
//		return luax_checktype<Shader>(L, idx, "Shader", GRAPHICS_PIXELEFFECT_T);
//	}
//
//	static const luaL_Reg functions[] = {
//		{ 0, 0 }
//	};
//
//	extern "C" int luaopen_shader(lua_State * L)
//	{
//		return luax_register_type(L, "Shader", functions);
//	}
//
//} // opengl
//} // graphics
//} // love