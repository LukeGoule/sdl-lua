#include "ECheckKey.h"

std::string ECheckKey::getName() {
	return std::string( "E_CheckKey" );
}

int ECheckKey::handle(lua_State* L) {
	if (lua_gettop(L) != 1) {
		this->m_engine->Error("E_CheckKey requires 1 argument.");
		return 1;
	}

	auto keyCode = static_cast<SDL_Keycode>(lua_tonumber(L, 1));
	lua_pushboolean(L, this->m_engine->getKeyboard()->checkState(keyCode));

	return 1;
}