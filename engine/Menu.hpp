#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include "EngineModule.hpp"
#include "SDL_events.h"

class TextEditor;

/*
 *
 * Our keyboard controller class. This handles the states of the keys on your keyboard.
 *
 */
class Menu : public EngineModule {
private:
	/*
	 * Vertex code editor component.
	 */
	TextEditor* m_pVertexEditor = nullptr;

	/*
	 * Fragment code editor component.
	 */
	TextEditor* m_pFragmentEditor = nullptr;

public:

	/*
	 * Constructor.
	 */
	Menu(Engine* instance);

	/*
	 * Destroy the object.
	 */
	~Menu();

	/*
	 * Initialise menu resources.
	 */
	void init();

	/*
	 * Initialise for OpenGL.
	 */
	void initOpenGL();

	/*
	 * Render the ImGui menu.
	 */
	void render();

	/*
	 * Handle an SDL event.
	 */
	void handleSDLEvent(SDL_Event*);

	/*
	 * Get pointer to vertex editor component.
	 */
	TextEditor* getVertexEditor();
	
	/*
	 * Get pointer to fragment editor component.
	 */
	TextEditor* getFragmentEditor();
};

#endif