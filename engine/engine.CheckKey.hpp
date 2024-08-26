#pragma once

#ifndef ENGINE_CHECK_KEY_HPP
#define ENGINE_CHECK_KEY_HPP

#include "BaseLuaFunction.hpp"

class engine_CheckKey : public BaseLuaFunction<engine_CheckKey> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_CHECK_KEY_HPP