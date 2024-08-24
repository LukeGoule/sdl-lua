#pragma once

#ifndef LUA_SETUP_WINDOW
#define LUA_SETUP_WINDOW

#include "BaseLuaFunction.h"

class engine_LaunchWindow : public BaseLuaFunction<engine_LaunchWindow> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_SETUP_WINDOW
