#include "engine.BindTexture.hpp"

std::string engine_BindTexture::getName() {
	return std::string("BindTexture");
}

int engine_BindTexture::handle(lua_State* L) {
	if (lua_gettop(L) != 2) {
		this->m_engine->Error("BindTexture requires 1 argument.");
		return 1;
	}

	glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(lua_tonumber(L, 2)));
	return 0;
}