#include "engine.GetMousePosition.hpp"

std::string engine_GetMousePosition::getName() {
	return std::string("GetMousePosition");
}

int engine_GetMousePosition::handle(lua_State* L) {
	int mx = 0, my = 0;

	SDL_GetMouseState(&mx, &my);

	lua_newtable(L);

	lua_pushnumber(L, static_cast<lua_Number>(mx));
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, static_cast<lua_Number>(my));
	lua_rawseti(L, -2, 2);

	return 1;
}