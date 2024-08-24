#include "Engine.hpp"
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