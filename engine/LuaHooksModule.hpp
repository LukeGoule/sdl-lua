#pragma once

#ifndef LUA_HOOKS_MODULE_HPP
#define LUA_HOOKS_MODULE_HPP

#include "BaseLuaModule.hpp"

template <typename T>
class BaseLuaFunction;

class LuaHooksModule : public BaseLuaModule<LuaHooksModule> {
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

#endif // !LUA_HOOKS_MODULE_HPP