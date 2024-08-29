#include "Hooks.hpp"
#include "Engine.hpp"

#include <lua.h>

Hooks::Hooks(Engine* instance) : EngineModule(instance) {
	this->m_vecHooks = std::vector<Hooks::Hook_t*>(100);
}

Hooks::~Hooks() {
}

void Hooks::init() {

} 
void Hooks::addHook(const char* szHookType, const char* szLuaGlobal, const char* szLuaName) {
	this->m_vecHooks[this->m_iHookIndex++] = new Hooks::Hook_t{
		szHookType,
		szLuaGlobal,
		szLuaName
	};
}

void Hooks::addHook(Hooks::Hook_t* hook) {
	this->m_vecHooks[this->m_iHookIndex++] = hook;
}

void Hooks::callHooks(const char* szHookType) {
	for (const auto& hook : this->m_vecHooks) {
		if ((!hook) || (!szHookType) || ((std::string(szHookType) != std::string(hook->m_szHookType)))) {
			continue;
		}

		this->callLuaMethod(hook->m_szLuaGlobal);
	}
}

void Hooks::deleteHook(const char* szLuaName) {
	// todo.
}

void Hooks::clearHooks(const char* szHookType) {
	// todo.
}

int Hooks::callLuaMethod(const char* szLuaGlobal) {
	lua_getglobal(this->m_engine->getLua(), szLuaGlobal);

	int lua_result = lua_pcall(this->m_engine->getLua(), 0, 0, 0);

	if (lua_result != 0) {
		printf("Error while attempting to call global method \"%s\": %s\n", szLuaGlobal, lua_tostring(this->m_engine->getLua(), -1));
	}

	return lua_result;
}