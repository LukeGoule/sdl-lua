#pragma once

#ifndef HOOKS_ADD_HPP
#define HOOKS_ADD_HPP

#include "BaseLuaFunction.hpp"

class hooks_Add : public BaseLuaFunction<hooks_Add> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !HOOKS_ADD_HPP
