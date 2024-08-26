#pragma once

#ifndef ENGINE_SETUP_WINDOW_HPP
#define ENGINE_SETUP_WINDOW_HPP

#include "BaseLuaFunction.hpp"

class engine_LaunchWindow : public BaseLuaFunction<engine_LaunchWindow> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_SETUP_WINDOW_HPP
