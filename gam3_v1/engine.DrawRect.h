#pragma once

#ifndef LUA_DRAW_RECT
#define LUA_DRAW_RECT

#include "BaseLuaFunction.h"

class engine_DrawRect : public BaseLuaFunction<engine_DrawRect> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_DRAW_RECT
