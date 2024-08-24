#include "engine.Error.h"

std::string engine_Error::getName() {
	return std::string("Error");
}

int engine_Error::handle(lua_State* L) {
	if (lua_gettop(L) != 2) {
		this->m_engine->Error("Error requires 1 argument.");
		return 1;
	}

	std::string theError = std::string(lua_tostring(L, 2));
	this->m_engine->Error(theError);

	return 1;
}