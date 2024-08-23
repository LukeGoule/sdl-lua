#include "EAtan2.h"

std::string EAtan2::getName() {
	return std::string("E_Atan2");
}

int EAtan2::handle( lua_State *L ) {
	double y = lua_tonumber(L, 1);
	double x = lua_tonumber(L, 2);

	lua_pushnumber(L, atan2(y, x));

	return 1;
}