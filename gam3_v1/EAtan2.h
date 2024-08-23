#pragma once

#ifndef LUA_ATAN2
#define LUA_ATAN2

#include "BaseLuaFunction.h"

class EAtan2 : public BaseLuaFunction<EAtan2> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_ATAN2
