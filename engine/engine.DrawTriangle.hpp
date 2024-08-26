#pragma once

#ifndef ENGINE_DRAW_TRIANGLE_HPP
#define ENGINE_DRAW_TRIANGLE_HPP

#include "BaseLuaFunction.hpp"

class engine_DrawTriangle : public BaseLuaFunction<engine_DrawTriangle> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_DRAW_TRIANGLE_HPP
