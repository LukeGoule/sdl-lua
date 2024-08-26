#include "engine.LaunchWindow.hpp"

std::string engine_LaunchWindow::getName() {
	return std::string("LaunchWindow");
}

int engine_LaunchWindow::handle(lua_State* L) {
	if (lua_gettop(L) != 4) {
		this->m_engine->Error("LaunchWindow requires 3 arguments.");
		return 1;
	}

	auto wx = static_cast<int>(lua_tonumber(L, 2));
	auto wy = static_cast<int>(lua_tonumber(L, 3));
	const char* wn = lua_tostring(L, 3);

	this->m_engine->getRender()->initSDL();
	this->m_engine->getRender()->initOpenGL();
	this->m_engine->getRender()->initWindow(wn, wx, wy);
	this->m_engine->getRender()->init2d(wx, wy);
	this->m_engine->getRender()->run2d();
	this->m_engine->getRender()->quit();

	return 0;
}