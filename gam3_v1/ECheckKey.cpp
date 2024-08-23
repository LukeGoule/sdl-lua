#include "ECheckKey.h"

std::string ECheckKey::getName() {
	return std::string( "E_CheckKey" );
}

int ECheckKey::handle( lua_State* L ) {
	double keyCode = lua_tonumber( L, 1 );
	
	lua_pushboolean( 
		L, 
		this->m_engine->m_keyboard->checkState( keyCode ) 
	);

	return 1;
}