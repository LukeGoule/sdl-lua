#pragma once

#ifndef LUA_LOAD_TEXTURE
#define LUA_LOAD_TEXTURE

#include "BaseLuaFunction.h"

class ELoadTexture : public BaseLuaFunction<ELoadTexture> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !LUA_LOAD_TEXTURE
