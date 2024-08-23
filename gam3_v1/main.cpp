// Statically assigned classes should be used in this engine to allow Lua
// to bind to the C(++) code.

#include "Engine.h"

#undef main
int main(int argc, char** argv) {
	
	const auto e = new Engine;
	e->Init();
	e->LoadScript("lua_base/main.lua");

#ifdef _DEBUG
	getchar();
#endif
}