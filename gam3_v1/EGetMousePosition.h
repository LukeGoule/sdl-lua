#pragma once

#ifndef LUA_GET_MOUSE_POSITION
#define LUA_GET_MOUSE_POSITION

#include "BaseLuaFunction.h"

class EGetMousePosition : public BaseLuaFunction<EGetMousePosition> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_GET_MOUSE_POSITION
