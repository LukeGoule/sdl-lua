#pragma once

#ifndef HOOKS_HPP
#define HOOKS_HPP

#include "EngineModule.hpp"

#include <iostream>
#include <vector>

/*
 *
 * Hooks controller class, for Lua.
 *
 */
class Hooks : public EngineModule {
public:

	struct Hook_t {
		const char* m_szHookType;
		const char* m_szLuaGlobal;
		const char* m_szLuaName;
	};

private:

	std::vector<Hooks::Hook_t*> m_vecHooks;
	int m_iHookIndex = 0;

public:

	/*
	 * Constructor.
	 */
	Hooks(Engine* instance);

	/*
	 * Destroy the object.
	 */
	~Hooks();

	/*
	 * Initialise menu resources.
	 */
	void init();

	/*
	 * Add a lua global hook.
	 */
	void addHook(const char* szHookType, const char* szLuaGlobal, const char* szLuaName);
	void addHook(Hooks::Hook_t* hook);

	/*
	 * Call all hook callbacks for a given type. 
	 */
	void callHooks(const char* szHookType);

	/*
	 * Delete a hook by name.
	 */
	void deleteHook(const char* szLuaName);

	/*
	 * Delete a hook by name.
	 */
	void clearHooks(const char* szHookType);

private:

	/*
	 * Call a given Lua method, using the Engine instance.
	 */
	int callLuaMethod(const char* szLuaGlobal);
};

#endif