#include "EAtan2.h"

std::string EAtan2::getName() {
	return std::string("E_Atan2");
}

int EAtan2::handle( lua_State *L ) {
	if (lua_gettop(L) != 1) {
		this->m_engine->Error("E_Atan2 requires 2 arguments.");
		return 1;
	}

	lua_Number y = lua_tonumber(L, 1);
	lua_Number x = lua_tonumber(L, 2);
	lua_pushnumber(L, atan2(static_cast<double>(y), static_cast<double>(x)));

	return 1;
}