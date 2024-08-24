#include "engine.DrawRect.h"

std::string engine_DrawRect::getName() {
	return std::string("DrawRect");
}

int engine_DrawRect::handle(lua_State* L) {
	if (lua_gettop(L) != 5) {
		this->m_engine->Error("DrawRect expects 4 arguments.");
		return 1;
	}

	auto x = static_cast<int>(lua_tonumber(L, 2)); // arg1
	auto y = static_cast<int>(lua_tonumber(L, 3)); // arg2
	auto w = static_cast<int>(lua_tonumber(L, 4)); // arg3
	auto h = static_cast<int>(lua_tonumber(L, 5)); // arg4

	/*
	To explain how this binds textures correctly,
	look at this diagram and then look at the code below.
	Each "TexCoord" binds a side of the shape to the texture.
	----------
	|A		B|
	|		 |
	|		 |
	|C		D|
	----------
	*/
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);			// A
	glVertex2i(x, y);
	glTexCoord2i(1, 0);			// B
	glVertex2i(x + w, y);
	glTexCoord2i(1, 1);			// D
	glVertex2i(x + w, y + h);
	glTexCoord2i(0, 1);			// C
	glVertex2i(x, y + h);
	glEnd();

	return 1;
}