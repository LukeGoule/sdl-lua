#pragma once

#ifndef ENGINE_DRAW_RECT_HPP
#define ENGINE_DRAW_RECT_HPP

#include "BaseLuaFunction.hpp"

class engine_DrawRect : public BaseLuaFunction<engine_DrawRect> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_DRAW_RECT_HPP
