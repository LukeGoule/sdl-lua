#pragma once

#ifndef LUA_SET_DRAW_COLOUR
#define LUA_SET_DRAW_COLOUR

#include "BaseLuaFunction.h"

class ESetDrawColour : public BaseLuaFunction<ESetDrawColour> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_SET_DRAW_COLOUR
