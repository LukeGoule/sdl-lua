#include "Engine.h"

#include <SDL_image.h>

lua_State*		Engine::L = nullptr;
SDL_Window*		Engine::pWin = nullptr;
SDL_GLContext	Engine::Context = NULL;
G3Keyboard*		Engine::m_keyboard = NULL;
bool			Engine::bShouldContinue = true;
int				Engine::iFullscreenMode = 0;

static int E_Atan2(lua_State* L) {
	double y = lua_tonumber(L, 1);
	double x = lua_tonumber(L, 2);

	lua_pushnumber(L, atan2(y, x));

	return 1;
}

static int E_CheckKey(lua_State* L) {
	double keyCode = lua_tonumber(L, 1);
	bool state = Engine::m_keyboard->checkState(keyCode);
	lua_pushboolean(L, state);
	return 1;
}

void Engine::Init() {
	Engine::Setup();

	lua_pushcfunction(Engine::L, Engine::SetupWindow);
	lua_setglobal(Engine::L, "E_LaunchWindow");

	lua_pushcfunction(Engine::L, Engine::SetDrawColour);
	lua_setglobal(Engine::L, "E_SetDrawColour");

	lua_pushcfunction(Engine::L, Engine::LoadTexture);
	lua_setglobal(Engine::L, "E_LoadTexture");

	lua_pushcfunction(Engine::L, Engine::DrawTriangle);
	lua_setglobal(Engine::L, "E_DrawTriangle");

	lua_pushcfunction(Engine::L, Engine::DrawRect);
	lua_setglobal(Engine::L, "E_DrawBasicRect");

	lua_pushcfunction(Engine::L, Engine::BindTexture);
	lua_setglobal(Engine::L, "E_BindTexture");

	lua_pushcfunction(Engine::L, Engine::SetFullscreen);
	lua_setglobal(Engine::L, "E_SetFullscreen");

	lua_pushcfunction(Engine::L, Engine::GetMousePosition);
	lua_setglobal(Engine::L, "E_GetMousePosition");

	lua_pushcfunction(Engine::L, Engine::Lua_CauseError);
	lua_setglobal(Engine::L, "E_Error");

	lua_pushcfunction(Engine::L, E_Atan2);
	lua_setglobal(Engine::L, "E_Atan2");

	lua_pushcfunction(Engine::L, E_CheckKey);
	lua_setglobal(Engine::L, "E_CheckKey");
}

void Engine::LoadScript(std::string script) {
	// Load the engine files

	auto fLoad = luaL_loadfile(Engine::L, script.c_str());

	if (fLoad) {
		Engine::LuaError("luaL_loadfile");

		return;
	}

	auto pCall = lua_pcall(Engine::L, 0, 0, 0);

	if (pCall) {
		Engine::LuaError("lua_pcall");

		return;
	}
}

void Engine::Setup() {
	Engine::L = luaL_newstate();
	luaL_openlibs(Engine::L);

	Engine::m_keyboard = new G3Keyboard;
}

void Engine::Error(std::string err) {
	printf("Error caused: %s\n", err.c_str());

	SDL_Quit();
}

void Engine::LuaError(std::string trace) {
	printf("[%s][LUA]: %s\n", trace.c_str(), lua_tostring(Engine::L, -1));

	SDL_Quit();
}

int Engine::RenderBinding(lua_State* L) {
	return 0;
}

int Engine::SetupWindow(lua_State* L) {

	int			wx = lua_tonumber(L, 1);
	int			wy = lua_tonumber(L, 2);
	const char* wn = lua_tostring(L, 3);

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Engine::pWin = SDL_CreateWindow(wn, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wx, wy, SDL_WINDOW_OPENGL);
	if (Engine::pWin == NULL) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	Engine::Context = SDL_GL_CreateContext(Engine::pWin);
	if (Engine::Context == NULL) {
		printf("SDL_Error: %s\n", SDL_GetError());
		return 0;
	}

	lua_getglobal(Engine::L, "Engine_LoadResources");

	if (lua_pcall(Engine::L, 0, 0, 0) != 0) {
		printf("[LUA], Engine_Render: %s\n", lua_tostring(L, -1));
		Engine::bShouldContinue = false;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glOrtho(0, wx, wy, 0, -1, 1);

	while (Engine::bShouldContinue) {
		SDL_Event* currEvents = new SDL_Event;
		while (SDL_PollEvent(currEvents)) {
			switch (currEvents->type) {
			case SDL_EventType::SDL_QUIT: // app quit signal.
				Engine::bShouldContinue = false;
				break;
			case SDL_EventType::SDL_KEYDOWN: // keyboard handler up.
				Engine::m_keyboard->setState(currEvents->key.keysym.sym, true);
				break;
			case SDL_EventType::SDL_KEYUP: // keyboard handler down.
				Engine::m_keyboard->setState(currEvents->key.keysym.sym, false);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		lua_getglobal(Engine::L, "Engine_Render");

		if (lua_pcall(Engine::L, 0, 0, 0) != 0) {
			printf("[LUA], Engine_Render: %s\n", lua_tostring(L, -1));
			Engine::bShouldContinue = false;
		}

		SDL_GL_SwapWindow(Engine::pWin);
	}

	SDL_Quit();

	return 0;
}

int Engine::SetDrawColour(lua_State* L) {
	if (lua_gettop(L) != 3) {
		Engine::Error("SetDrawColour expects 3 arguments");
		return 1;
	}

	glColor3d(lua_tonumber(L, 1), lua_tonumber(L, 2), lua_tonumber(L, 3));
}

// Draw a simple rectangle, this can have images bound to it.
int Engine::DrawRect(lua_State* L) {
	if (lua_gettop(L) != 4) {
		Engine::Error("DrawRect expects 4 arguments.");
		return 1;
	}

	int x = lua_tonumber(L, 1); // arg1
	int y = lua_tonumber(L, 2); // arg2
	int w = lua_tonumber(L, 3); // arg3
	int h = lua_tonumber(L, 4); // arg4

	/*
	To explain how this binds textures correctly,
	look at this diagram and then look at the code below.
	Each "TexCoord" binds a side of the shape to the texture.
	----------
	|A		B|
	|		 |
	|		 |
	|C		D|
	----------
	*/

	glBegin(GL_QUADS);
	{
		glTexCoord2i(0, 0);			// A
		glVertex2i(x, y);

		glTexCoord2i(1, 0);			// B
		glVertex2i(x + w, y);

		glTexCoord2i(1, 1);			// D
		glVertex2i(x + w, y + h);

		glTexCoord2i(0, 1);			// C
		glVertex2i(x, y + h);
	}
	glEnd();

	return 1;
}

// Draw a simple triangle.
int Engine::DrawTriangle(lua_State* L) {
	if (lua_gettop(L) != 6) {
		Engine::Error("DrawTriangle expects 3 arguments.");
		return 1;
	}

	int x1 = lua_tonumber(L, 1);
	int y1 = lua_tonumber(L, 2);
	int x2 = lua_tonumber(L, 3);
	int y2 = lua_tonumber(L, 4);
	int x3 = lua_tonumber(L, 5);
	int y3 = lua_tonumber(L, 6);

	glBegin(GL_TRIANGLES);
	{
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	}
	glEnd();

	return 1;
}

/*
This function loads up a texture into the GPU, ready to be used.
Thanks to: http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
*/
int Engine::LoadTexture(lua_State* L) {
	if (lua_gettop(L) != 1) {
		Engine::Error("LoadTexture requires 1 argument.");
		return 1;
	}

	GLuint TextureID = 0;

	SDL_Surface* Surface = IMG_Load(lua_tostring(L, 1));

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	int Mode = GL_RGB;

	if (Surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	lua_pushinteger(L, TextureID);

	return 1;
}

int Engine::BindTexture(lua_State* L) {
	if (lua_gettop(L) != 1) {
		Engine::Error("BindTexture requires 1 argument.");
		return 1;
	}

	glBindTexture(GL_TEXTURE_2D, lua_tonumber(L, 1));
}

int Engine::SetFullscreen(lua_State* L) {
	// SDL_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN_DESKTOP or 0

	if (lua_gettop(L) != 1) {
		Engine::Error("SetFullscreen requires 1 argument\n");
	}

	int OldMode = Engine::iFullscreenMode;

	Engine::iFullscreenMode = (int)lua_tointeger(Engine::L, 1);

	if (!(Engine::iFullscreenMode == 0 || Engine::iFullscreenMode == SDL_WINDOW_FULLSCREEN || Engine::iFullscreenMode == SDL_WINDOW_FULLSCREEN_DESKTOP )) {
		Engine::Error("Invalid fullscreen mode passed!\n");
	}

	SDL_SetWindowFullscreen(Engine::pWin, Engine::iFullscreenMode);

	return 1;
}
int Engine::GetMousePosition(lua_State* L) {
	int mx = 0, my = 0;

	SDL_GetMouseState(&mx, &my);

	lua_newtable(L);

	lua_pushnumber(L, mx);
	lua_rawseti(L, -2, 1);

	lua_pushnumber(L, my);
	lua_rawseti(L, -2, 2);

	return 1;
}

int Engine::Lua_CauseError(lua_State* L) {
	std::string theError = std::string(lua_tostring(L, 1));
	Engine::Error(theError);

	return 1;
}