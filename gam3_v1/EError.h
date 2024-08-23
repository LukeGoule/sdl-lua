#pragma once

#ifndef LUA_E_ERROR
#define LUA_E_ERROR

#include "BaseLuaFunction.h"

class EError : public BaseLuaFunction<EError> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_E_ERROR
