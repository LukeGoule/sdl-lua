#include "LuaImGuiModule.hpp"
#include "imgui.Begin.hpp"
#include "imgui.End.hpp"

std::string LuaImGuiModule::getName() {
	return std::string("imgui");
}


std::vector<LuaRegisterFunc> LuaImGuiModule::getMethods() {
	return std::vector<LuaRegisterFunc> {
		imgui_Begin::luaRegister,
		imgui_End::luaRegister
	};
}