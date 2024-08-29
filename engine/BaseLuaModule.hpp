#pragma once

#ifndef BASE_LUA_MODULE_HPP
#define BASE_LUA_MODULE_HPP

#include <iostream>
#include <vector>

#include "lua.hpp"
#include "Engine.hpp"

typedef void (*LuaRegisterFunc)(Engine*);

/*
 * Base class defining engine-bindable lua module.
 */
template <typename T>
class BaseLuaModule {
public:
	/*
	 * Reference to base engine class. Allows for quick access via this->m_engine.
	 */
	Engine* m_engine = nullptr;

	/*
	 * Deleted copy constructor and assignment operator to prevent copying.
	 */
	BaseLuaModule(const BaseLuaModule&) = delete;

	/*
	 * Deleted copy constructor and assignment operator to prevent copying.
	 */
	BaseLuaModule& operator=(const BaseLuaModule&) = delete;

	/*
	 * Get the name of the module. This declares how to require() it in Lua. 
	 */
	virtual std::string getName() = 0;
	
	/*
	 * Get the methods of the module.
	 */
	virtual std::vector<LuaRegisterFunc> getMethods() = 0;

	/*
	 * Register the module within a Lua state.
	 */
	static int luaRegister(lua_State* L) {
		// Create a new table, which will be require-able.
		lua_newtable(L);

		const auto instance = &T::getInstance();

		for (auto var : instance->getMethods()) {
			var(&Engine::getInstance());
		}

		// The table is already on the top of the stack, so return it
		return 1;
	}

	/*
	 * Add the module to the Engine Lua instance.
	 */
	inline void addModule(Engine* engine) {
		// Register the test module
		luaL_requiref(engine->getLua(), this->getName().c_str(), T::luaRegister, 1);
		lua_pop(engine->getLua(), 1);
	}

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

protected:
	/*
	 * Protected constructor and destructor to prevent direct instantiation.
	 */
	BaseLuaModule() {}

	/*
	 * Protected constructor and destructor to prevent direct instantiation.
	 */
	virtual ~BaseLuaModule() {}

	/*
	 * Friend declaration to allow access to the protected constructor.
	 */
	friend T;
};

#endif // !BASE_LUA_MODULE_HPP