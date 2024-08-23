#pragma once

#ifndef LUA_DRAW_RECT
#define LUA_DRAW_RECT

#include "BaseLuaFunction.h"

class EDrawRect : public BaseLuaFunction<EDrawRect> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_DRAW_RECT
