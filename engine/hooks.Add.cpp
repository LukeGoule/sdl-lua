#include "hooks.Add.hpp"

std::string hooks_Add::getName() {
	return std::string("Add");
}

int hooks_Add::handle(lua_State* L) {
	if (lua_gettop(L) != (int)3) {
		this->m_engine->Error("hooks.Add requires 3 arguments.");
		return 1;
	}

	const auto szHookType = lua_tostring(L, 1);
	const auto szHookGlobal = lua_tostring(L, 2);
	const auto szHookName = lua_tostring(L, 3);

	const auto engine = &Engine::getInstance();
	engine->getHooks()->addHook(new Hooks::Hook_t{
		szHookType,
		szHookGlobal,
		szHookName
	});

	return 0;
}