#pragma once

#ifndef ENGINE_ATAN2_HPP
#define ENGINE_ATAN2_HPP

#include "BaseLuaFunction.hpp"

class engine_Atan2 : public BaseLuaFunction<engine_Atan2> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_ATAN2_HPP
