#include "engine.SetDrawColour.hpp"

std::string engine_SetDrawColour::getName() {
	return std::string("SetDrawColour");
}

int engine_SetDrawColour::handle( lua_State* L ) {
	if ( lua_gettop(L) != 4 ) {
		this->m_engine->Error("SetDrawColour expects 3 arguments");
		return 0;
	}

	glColor3d( lua_tonumber( L, 2 ), lua_tonumber( L, 3 ), lua_tonumber( L, 4 ) );

	return 0;
}