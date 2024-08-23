#include "ESetupWindow.h"

std::string ESetupWindow::getName() {
	return std::string("E_LaunchWindow");
}

int ESetupWindow::handle(lua_State* L) {
	if (lua_gettop(L) != 3) {
		this->m_engine->Error("E_LaunchWindow requires 3 arguments.");
		return 1;
	}

	auto wx = static_cast<int>(lua_tonumber(L, 1));
	auto wy = static_cast<int>(lua_tonumber(L, 2));
	const char* wn = lua_tostring(L, 3);

	this->m_engine->getRender()->initSDL();
	this->m_engine->getRender()->initOpenGL();
	this->m_engine->getRender()->initWindow(wn, wx, wy);
	this->m_engine->getRender()->init2d(wx, wy);
	this->m_engine->getRender()->run();
	this->m_engine->getRender()->quit();

	return 0;
}