#pragma once

#ifndef ENGINE_SET_DRAW_COLOUR_HPP
#define ENGINE_SET_DRAW_COLOUR_HPP

#include "BaseLuaFunction.hpp"

class engine_SetDrawColour : public BaseLuaFunction<engine_SetDrawColour> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_SET_DRAW_COLOUR_HPP
