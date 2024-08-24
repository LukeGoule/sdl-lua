#pragma once

#ifndef ENGINE_MODULE_H
#define ENGINE_MODULE_H

class Engine;

/*
 * Class that all engine "modules" should derive from. Defines common way of interacting with our system.
 */
class EngineModule {
protected:
	/*
	 * The Engine instance.
	 * Engine is a singleton, but access via m_engine is far easier to write.
	 */
	Engine* m_engine;
public:
	/*
	 * Initialise our class, setting the Engine instance.
	 */
	EngineModule(Engine* instance) {
		this->m_engine = instance;
	}
};

#endif