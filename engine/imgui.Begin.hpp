#pragma once

#ifndef IMGUI_BEGIN_HPP
#define IMGUI_BEGIN_HPP

#include "BaseLuaFunction.hpp"

class imgui_Begin : public BaseLuaFunction<imgui_Begin> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !IMGUI_BEGIN_HPP
