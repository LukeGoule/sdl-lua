#include "ESetDrawColour.h"

std::string ESetDrawColour::getName() {
	return std::string("E_SetDrawColour");
}

int ESetDrawColour::handle( lua_State* L ) {
	if ( lua_gettop(L) != 3 ) {
		this->m_engine->Error("E_SetDrawColour expects 3 arguments");
		return 0;
	}

	glColor3d( lua_tonumber( L, 1 ), lua_tonumber( L, 2 ), lua_tonumber( L, 3 ) );

	return 0;
}