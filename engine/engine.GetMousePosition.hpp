#pragma once

#ifndef ENGINE_GET_MOUSE_POSITION_HPP
#define ENGINE_GET_MOUSE_POSITION_HPP

#include "BaseLuaFunction.hpp"

class engine_GetMousePosition : public BaseLuaFunction<engine_GetMousePosition> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_GET_MOUSE_POSITION_HPP
