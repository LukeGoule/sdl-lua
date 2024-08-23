#include "EDrawTriangle.h"

std::string EDrawTriangle::getName() {
	return std::string("E_DrawTriangle");
}

int EDrawTriangle::handle(lua_State* L) {
	if (lua_gettop(L) != 6) {
		this->m_engine->Error("E_DrawTriangle expects 6 arguments.");
		return 1;
	}

	auto x1 = static_cast<int>(lua_tonumber(L, 1));
	auto y1 = static_cast<int>(lua_tonumber(L, 2));
	auto x2 = static_cast<int>(lua_tonumber(L, 3));
	auto y2 = static_cast<int>(lua_tonumber(L, 4));
	auto x3 = static_cast<int>(lua_tonumber(L, 5));
	auto y3 = static_cast<int>(lua_tonumber(L, 6));

	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();

	return 1;
}