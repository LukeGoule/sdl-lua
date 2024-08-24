#pragma once

#ifndef LUA_ATAN2
#define LUA_ATAN2

#include "BaseLuaFunction.h"

class engine_Atan2 : public BaseLuaFunction<engine_Atan2> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_ATAN2
