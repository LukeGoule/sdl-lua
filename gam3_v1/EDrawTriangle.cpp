#include "EDrawTriangle.h"

std::string EDrawTriangle::getName() {
	return std::string("E_DrawTriangle");
}

int EDrawTriangle::handle(lua_State* L) {
	if (lua_gettop(L) != 6) {
		this->m_engine->Error("DrawTriangle expects 3 arguments.");
		return 1;
	}

	int x1 = lua_tonumber(L, 1);
	int y1 = lua_tonumber(L, 2);
	int x2 = lua_tonumber(L, 3);
	int y2 = lua_tonumber(L, 4);
	int x3 = lua_tonumber(L, 5);
	int y3 = lua_tonumber(L, 6);

	glBegin(GL_TRIANGLES);
	{
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	}
	glEnd();

	return 1;
}