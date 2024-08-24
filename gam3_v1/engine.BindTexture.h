#pragma once

#ifndef LUA_BIND_TEXTURE
#define LUA_BIND_TEXTURE

#include "BaseLuaFunction.h"

class engine_BindTexture : public BaseLuaFunction<engine_BindTexture> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_BIND_TEXTURE
