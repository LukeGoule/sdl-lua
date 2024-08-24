#include "engine.Atan2.hpp"

std::string engine_Atan2::getName() {
	return std::string("Atan2");
}

int engine_Atan2::handle( lua_State *L ) {
	if (lua_gettop(L) != 3) {
		this->m_engine->Error("Atan2 requires 2 arguments.");
		return 1;
	}

	lua_Number y = lua_tonumber(L, 2);
	lua_Number x = lua_tonumber(L, 3);
	lua_pushnumber(L, atan2(static_cast<double>(y), static_cast<double>(x)));

	return 1;
}