#pragma once

#ifndef LUA_FUNCTION_BASE_H
#define LUA_FUNCTION_BASE_H

#include <iostream>

#include "lua.hpp"
#include "Engine.h"

template <typename T>
class BaseLuaFunction {
public:
	Engine* m_engine = nullptr;

	// Deleted copy constructor and assignment operator to prevent copying
	BaseLuaFunction(const BaseLuaFunction&) = delete;
	BaseLuaFunction& operator=(const BaseLuaFunction&) = delete;

	// Static method to get the single instance of the class
	static T& getInstance() {
		static T instance;  // Guaranteed to be destroyed, instantiated on first use
		return instance;
	}
	
	virtual std::string getName() = 0;
	virtual int handle(lua_State *L) = 0;

	static int luaHandle(lua_State *L) {
		return BaseLuaFunction::getInstance().handle(L);
	}

	static void luaRegister(Engine* instance) {
		T::getInstance().m_engine = instance;

		lua_pushcfunction(instance->getLua(), T::luaHandle);
		lua_setfield(instance->getLua(), -2, T::getInstance().getName().c_str());
		//lua_setglobal(instance->getLua(), T::getInstance().getName().c_str());
	}

protected:
	// Protected constructor and destructor to prevent direct instantiation
	BaseLuaFunction() {}
	virtual ~BaseLuaFunction() {}

	// Friend declaration to allow access to the protected constructor
	friend T;
};

#endif