#include "imgui.End.hpp"
#include "../packages/ImGui/imgui.h"

std::string imgui_End::getName() {
	return std::string("End");
}

int imgui_End::handle(lua_State* L) {
	if (lua_gettop(L) != (int)1) {
		this->m_engine->Error("imgui.Begin takes zero arguments.");
		return 1;
	}

	ImGui::End();

	return 0;
}