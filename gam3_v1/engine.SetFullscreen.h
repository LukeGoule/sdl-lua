#pragma once

#ifndef LUA_SET_FULLSCREEN
#define LUA_SET_FULLSCREEN

#include "BaseLuaFunction.h"

class engine_SetFullscreen : public BaseLuaFunction<engine_SetFullscreen> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_SET_FULLSCREEN
