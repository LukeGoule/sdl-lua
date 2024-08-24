#pragma once

#ifndef LUA_SET_DRAW_COLOUR
#define LUA_SET_DRAW_COLOUR

#include "BaseLuaFunction.h"

class engine_SetDrawColour : public BaseLuaFunction<engine_SetDrawColour> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_SET_DRAW_COLOUR
