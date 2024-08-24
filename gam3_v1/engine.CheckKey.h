#pragma once
#pragma once

#ifndef LUA_CHECK_KEY
#define LUA_CHECK_KEY

#include "BaseLuaFunction.h"

class engine_CheckKey : public BaseLuaFunction<engine_CheckKey> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_ATAN2