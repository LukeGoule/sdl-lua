#pragma once

#ifndef LUA_IM_GUI_MODULE_HPP
#define LUA_IM_GUI_MODULE_HPP

#include "BaseLuaModule.hpp"

template <typename T>
class BaseLuaFunction;

class LuaImGuiModule : public BaseLuaModule<LuaImGuiModule> {
public:

	/*
	 * Get the name of the module. This declares how to require() it in Lua.
	 */
	std::string getName() override;

	/*
	 * Get the methods of the module.
	 */
	std::vector<LuaRegisterFunc> getMethods() override;
};

#endif // !LUA_IM_GUI_MODULE_HPP