#include "EBindTexture.h"

std::string EBindTexture::getName() {
	return std::string("E_BindTexture");
}

int EBindTexture::handle(lua_State* L) {
	if (lua_gettop(L) != 1) {
		this->m_engine->Error("E_BindTexture requires 1 argument.");
		return 1;
	}

	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(lua_tonumber(L, 1)));
	return 0;
}