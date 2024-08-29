#pragma once

#ifndef IMGUI_END_HPP
#define IMGUI_END_HPP

#include "BaseLuaFunction.hpp"

class imgui_End : public BaseLuaFunction<imgui_End> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !IMGUI_END_HPP
