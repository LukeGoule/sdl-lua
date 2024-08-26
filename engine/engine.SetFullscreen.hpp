#pragma once

#ifndef ENGINE_SET_FULLSCREEN_HPP
#define ENGINE_SET_FULLSCREEN_HPP

#include "BaseLuaFunction.hpp"

class engine_SetFullscreen : public BaseLuaFunction<engine_SetFullscreen> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_SET_FULLSCREEN_HPP
