// Statically assigned classes should be used in this engine to allow Lua
// to bind to the C(++) code.

#include "Engine.h"

#undef main
int main(int argc, char** argv) {

	Engine::Init();

	Engine::LoadScript("lua/main.lua");

#ifdef _DEBUG
	getchar();
#endif
}