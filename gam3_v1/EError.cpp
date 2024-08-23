#include "EError.h"

std::string EError::getName() {
	return std::string("E_Error");
}

int EError::handle(lua_State* L) {
	if (lua_gettop(L) != 1) {
		this->m_engine->Error("E_Error requires 1 argument.");
		return 1;
	}

	std::string theError = std::string(lua_tostring(L, 1));
	this->m_engine->Error(theError);

	return 1;
}