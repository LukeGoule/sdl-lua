#include "engine.DrawTriangle.hpp"

std::string engine_DrawTriangle::getName() {
	return std::string("DrawTriangle");
}

int engine_DrawTriangle::handle(lua_State* L) {
	if (lua_gettop(L) != 7) {
		this->m_engine->Error("DrawTriangle expects 6 arguments.");
		return 1;
	}

	auto x1 = static_cast<int>(lua_tonumber(L, 2));
	auto y1 = static_cast<int>(lua_tonumber(L, 3));
	auto x2 = static_cast<int>(lua_tonumber(L, 4));
	auto y2 = static_cast<int>(lua_tonumber(L, 5));
	auto x3 = static_cast<int>(lua_tonumber(L, 6));
	auto y3 = static_cast<int>(lua_tonumber(L, 7));

	glBegin(GL_TRIANGLES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glEnd();

	return 1;
}