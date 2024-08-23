#pragma once

#ifndef LUA_DRAW_TRIANGLE
#define LUA_DRAW_TRIANGLE

#include "BaseLuaFunction.h"

class EDrawTriangle : public BaseLuaFunction<EDrawTriangle> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_DRAW_TRIANGLE
