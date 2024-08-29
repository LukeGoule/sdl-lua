#include "LuaHooksModule.hpp"
#include "hooks.Add.hpp"

std::string LuaHooksModule::getName() {
	return std::string("hooks");
}

std::vector<LuaRegisterFunc> LuaHooksModule::getMethods() {
	return std::vector<LuaRegisterFunc> {
		hooks_Add::luaRegister
	};
}