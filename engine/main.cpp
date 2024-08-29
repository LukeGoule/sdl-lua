#include "Engine.hpp"
#include "SDL.h"

#undef main

int main(int argc, char** argv) {
	(&Engine::getInstance())
		->Init()
		->LoadScript("lua_base/main.lua")
		->Cleanup();

	getchar();
}