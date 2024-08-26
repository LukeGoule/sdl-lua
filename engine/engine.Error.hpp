#pragma once

#ifndef ENGINE_ERROR_HPP
#define ENGINE_ERROR_HPP

#include "BaseLuaFunction.hpp"

class engine_Error : public BaseLuaFunction<engine_Error> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_ERROR_HPP
