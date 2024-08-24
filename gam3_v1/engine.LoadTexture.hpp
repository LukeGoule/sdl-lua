#pragma once

#ifndef ENGINE_LOAD_TEXTURE_HPP
#define ENGINE_LOAD_TEXTURE_HPP

#include "BaseLuaFunction.hpp"

class engine_LoadTexture : public BaseLuaFunction<engine_LoadTexture> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_LOAD_TEXTURE_HPP
