#pragma once

#ifndef ENGINE_BIND_TEXTURE_HPP
#define ENGINE_BIND_TEXTURE_HPP

#include "BaseLuaFunction.hpp"

class engine_BindTexture : public BaseLuaFunction<engine_BindTexture> {
public:
	std::string getName() override;
	int handle(lua_State* L) override;
};

#endif // !ENGINE_BIND_TEXTURE_HPP
