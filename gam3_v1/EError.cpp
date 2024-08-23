#include "EError.h"

std::string EError::getName() {
	return std::string("E_Error");
}

int EError::handle(lua_State* L) {
	std::string theError = std::string(lua_tostring(L, 1));
	this->m_engine->Error(theError);

	return 1;
}