#pragma once

#ifndef LUA_BIND_TEXTURE
#define LUA_BIND_TEXTURE

#include "BaseLuaFunction.h"

class EBindTexture : public BaseLuaFunction<EBindTexture> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_BIND_TEXTURE
