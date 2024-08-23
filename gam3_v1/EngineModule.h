#pragma once

#ifndef ENGINE_MODULE_H
#define ENGINE_MODULE_H

class Engine;

class EngineModule {
protected:
	Engine* m_engine;
public:
	EngineModule(Engine* instance) {
		this->m_engine = instance;
	}
};

#endif