// Statically assigned classes should be used in this engine to allow Lua
// to bind to the C(++) code.

#include "Engine.h"

#undef main
int main(int argc, char** argv) {
	const auto m_engine = &Engine::getInstance();
	
	m_engine
		->Init()
		->LoadScript("lua_base/main.lua");

#ifdef _DEBUG
	return getchar();
#else
	return 0;
#endif
}