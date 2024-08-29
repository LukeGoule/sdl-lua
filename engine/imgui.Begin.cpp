#include "imgui.Begin.hpp"
#include "../packages/ImGui/imgui.h"

std::string imgui_Begin::getName() {
	return std::string("Begin");
}

int imgui_Begin::handle(lua_State* L) {
	if (lua_gettop(L) != (int)2) {
		this->m_engine->Error("imgui.Begin requires 1 argument.");
		return 0;
	}

	const char* szTitle = lua_tostring(L, 2);
	ImGui::Begin(szTitle);

	return 0;
}