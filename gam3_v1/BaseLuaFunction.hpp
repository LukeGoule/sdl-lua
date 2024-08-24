#pragma once

#ifndef LUA_FUNCTION_BASE_H
#define LUA_FUNCTION_BASE_H

#include <iostream>

#include "lua.hpp"
#include "Engine.hpp"

/*
 * Base class defining engine-bindable lua methods.
 */
template <typename T>
class BaseLuaFunction {
public:
	/*
	 * Reference to base engine class. Allows for quick access via this->m_engine.
	 */
	Engine* m_engine = nullptr;

	/*
	 * Deleted copy constructor and assignment operator to prevent copying.
	 */
	BaseLuaFunction(const BaseLuaFunction&) = delete;

	/*
	 * Deleted copy constructor and assignment operator to prevent copying.
	 */
	BaseLuaFunction& operator=(const BaseLuaFunction&) = delete;

	/*
	 * Static method to get the single instance of the class.
	 * Call as;
	 * ```cpp
	 * const auto someFunction = &engine_SomeFunction::getInstance();
	 * ```
	 */
	static T& getInstance() {
		static T instance;  // Guaranteed to be destroyed, instantiated on first use.
		return instance;
	}
	
	/*
	 * Define the name of the function within the table.
	 */
	virtual std::string getName() = 0;

	/*
	 * Define the function callback for Lua.
	 */
	virtual int handle(lua_State *L) = 0;
	
	/*
	 * Static callback method for Lua. 
	 * This is called so we can get the instance of this object and call the callback in the base class.
	 */
	static int luaHandle(lua_State *L) {
		return T::getInstance().handle(L);
	}

	/*
	 * Register this function within the current table.
	 * We assume the current table is the `engine` module defined in Engine.cpp.
	 */
	static void luaRegister(Engine* instance) {
		// Set the m_engine instance.
		T::getInstance().m_engine = instance;

		// Push to stack.
		lua_pushcfunction(instance->getLua(), T::luaHandle);

		// Set function within current table, using the baseclass defined func name (getName()).
		lua_setfield(instance->getLua(), -2, T::getInstance().getName().c_str());
	}

protected:
	/*
	 * Protected constructor and destructor to prevent direct instantiation.
	 */
	BaseLuaFunction() {}

	/*
	 * Protected constructor and destructor to prevent direct instantiation.
	 */
	virtual ~BaseLuaFunction() {}

	/*
	 * Friend declaration to allow access to the protected constructor.
	 */
	friend T;
};

#endif