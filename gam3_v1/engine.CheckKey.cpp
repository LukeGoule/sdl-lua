#include "engine.CheckKey.h"

std::string engine_CheckKey::getName() {
	return std::string( "CheckKey" );
}

int engine_CheckKey::handle(lua_State* L) {
	if (lua_gettop(L) != 2) {
		this->m_engine->Error("CheckKey requires 1 argument.");
		return 1;
	}

	auto keyCode = static_cast<SDL_Keycode>(lua_tonumber(L, 2));
	lua_pushboolean(L, this->m_engine->getKeyboard()->checkState(keyCode));

	return 1;
}