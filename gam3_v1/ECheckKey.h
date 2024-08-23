#pragma once
#pragma once

#ifndef LUA_CHECK_KEY
#define LUA_CHECK_KEY

#include "BaseLuaFunction.h"

class ECheckKey : public BaseLuaFunction<ECheckKey> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_ATAN2
