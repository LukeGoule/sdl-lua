#include "EGetMousePosition.h"

std::string EGetMousePosition::getName() {
	return std::string("E_GetMousePosition");
}

int EGetMousePosition::handle(lua_State* L) {
	int mx = 0, my = 0;

	SDL_GetMouseState(&mx, &my);

	lua_newtable(L);

	lua_pushnumber(L, static_cast<lua_Number>(mx));
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, static_cast<lua_Number>(my));
	lua_rawseti(L, -2, 2);

	return 1;
}