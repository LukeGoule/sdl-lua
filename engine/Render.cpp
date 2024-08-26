#include "Render.hpp"
#include "Engine.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../ImGuiColorTextEdit/TextEditor.h"

#include <iostream>
#include <fstream>
#include <sstream>

std::string readFileIntoString(const std::string& filePath) {
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open()) {
		std::cerr << "Could not open the file: " << filePath << std::endl;
		return "";
	}

	std::stringstream buffer;
	buffer << fileStream.rdbuf();
	return buffer.str();
}

Render::Render(Engine* instance) : EngineModule(instance) {
	this->m_sVertex = readFileIntoString("shaders/vertex.glsl");
	this->m_sFragment = readFileIntoString("shaders/fragment.glsl");
}

Render::~Render() {}

void Render::initSDL() {
	SDL_Init(SDL_INIT_EVERYTHING);
}

void Render::initOpenGL() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	this->m_engine->getMenu()->init();
}

void Render::initWindow(const char* title, int widthPx, int heightPx) {
	this->m_screen_w = widthPx;
	this->m_screen_h = heightPx;
	this->m_window = SDL_CreateWindow(title , SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, widthPx, heightPx, SDL_WINDOW_OPENGL);
	
	if (this->m_window == NULL) {
		this->m_engine->Error(std::string("SDL_Error: ") + std::string(SDL_GetError()));
	}
}

void Render::init2d(int widthPx, int heightPx) {
	this->m_gl_context = SDL_GL_CreateContext(this->m_window);
	if (this->m_gl_context == NULL) {
		this->m_engine->Error(std::string("SDL_Error: ") + std::string(SDL_GetError()));
	}
}

void Render::run2d() {
	lua_getglobal(this->m_engine->getLua(), "Engine_LoadResources");

	if (lua_pcall(this->m_engine->getLua(), 0, 0, 0) != 0) {
		this->m_engine->Error(std::string("Engine_Render: ") + lua_tostring(this->m_engine->getLua(), -1));
		this->m_continue = false;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		this->m_engine->Error("GLEW could not be initialised.");
		this->m_continue = false;
	}

	// V-Sync
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->m_engine->getMenu()->initOpenGL();

	float vertices[] = {
		// Positions         // Colors
		 1000.f,  1000.f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top Right
		 1000.f, -1000.f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Right
		-1000.f, -1000.f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom Left
		-1000.f,  1000.f, 0.0f,  1.0f, 1.0f, 0.0f   // Top Left 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBO, VAO, EBO;
	this->m_iShaderProgram = this->compileShaderProgram(this->m_sVertex.c_str(), this->m_sFragment.c_str());
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Setup transformations here
	//float angle = glm::radians((float)(tick % 360));
	//model = glm::rotate<float>(model, angle, glm::vec3(0.0f, 1.0f, 1.0f));
	auto model		= glm::mat4(1.0f);
	auto view		= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

	while (this->m_continue) {
		SDL_Event* currEvents = new SDL_Event;
		while (SDL_PollEvent(currEvents)) {
			this->m_engine->getMenu()->handleSDLEvent(currEvents);

			switch (currEvents->type) {
			case SDL_EventType::SDL_QUIT: // app quit signal.
				this->m_continue = false;
				break;
			case SDL_EventType::SDL_KEYDOWN: // keyboard handler up.
				this->m_engine->getKeyboard()->setState(currEvents->key.keysym.sym, true);
				break;
			case SDL_EventType::SDL_KEYUP: // keyboard handler down.
				this->m_engine->getKeyboard()->setState(currEvents->key.keysym.sym, false);
				break;
			}
		}

		float currentTime = SDL_GetTicks() / 1000.0f; // Time in seconds
		int mx = 0, my = 0;
		SDL_GetMouseState(&mx, &my);

		auto projection = glm::perspective(glm::radians(45.0f), (float)this->m_screen_w / (float)this->m_screen_h, 0.1f, 100.0f);

		glViewport(0, 0, this->m_screen_w, this->m_screen_h);
		glOrtho(0, this->m_screen_w, this->m_screen_h, 0, -1, 1);

		glUseProgram(this->m_iShaderProgram);

		GLuint modelLoc = glGetUniformLocation(this->m_iShaderProgram, "m_mat4Model");
		GLuint viewLoc = glGetUniformLocation(this->m_iShaderProgram, "m_mat4View");
		GLuint projLoc = glGetUniformLocation(this->m_iShaderProgram, "m_mat4Projection");
		GLuint timeLoc = glGetUniformLocation(this->m_iShaderProgram, "m_iTime");
		GLuint resolutionLoc = glGetUniformLocation(this->m_iShaderProgram, "m_vec2Resolution");
		GLuint mousePosLoc = glGetUniformLocation(this->m_iShaderProgram, "m_vec2MousePosition");

		GLuint audioAmpsLoc = glGetUniformLocation(this->m_iShaderProgram, "m_vec3AudioAmplitude");

		glUniform1f(timeLoc, currentTime);
		glUniform2f(resolutionLoc, (float)this->m_screen_w, (float)this->m_screen_h);
		glUniform2f(mousePosLoc, (float)mx, (float)my);

		glUniform3f(audioAmpsLoc, 
			this->m_engine->getPortaudio()->getLow(),
			this->m_engine->getPortaudio()->getMid(), 
			this->m_engine->getPortaudio()->getHigh()
		);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//lua_getglobal(this->m_engine->getLua(), "Engine_Render");

		//if (lua_pcall(this->m_engine->getLua(), 0, 0, 0) != 0) {
		//	printf("[LUA], Engine_Render: %s\n", lua_tostring(this->m_engine->getLua(), -1));
		//	this->m_continue = false;
		//}
		

		// recompile shaders when F2 is pressed.
		if (this->m_engine->getKeyboard()->checkState(SDLK_F2)) {
			glDeleteProgram(this->m_iShaderProgram);

			this->recompileShaders(true); // Recompile from data files.
			this->m_engine->getMenu()->getVertexEditor()->SetText(this->m_sVertex);
			this->m_engine->getMenu()->getFragmentEditor()->SetText(this->m_sFragment);
		}

		if (this->m_engine->getKeyboard()->checkState(SDLK_F3)) {
			this->m_screen_w = 1920;
			this->m_screen_h = 1080;

			SDL_SetWindowSize(this->m_window, this->m_screen_w, this->m_screen_h);
			this->setFullscreenMode(SDL_WINDOW_FULLSCREEN_DESKTOP);
		}

		if (this->m_engine->getKeyboard()->checkState(SDLK_F4)) {
			this->m_screen_w = 1600;
			this->m_screen_h = 900;

			SDL_SetWindowSize(this->m_window, this->m_screen_w, this->m_screen_h);
			this->setFullscreenMode(0);
		}

		this->m_engine->getMenu()->render();

		
		SDL_GL_SwapWindow(this->m_window);
	}
}

void Render::quit() {
	SDL_Quit();
}

void Render::recompileShaders(bool bFromFiles) {
	if (bFromFiles) {
		this->m_sVertex = readFileIntoString("shaders/vertex.glsl");
		this->m_sFragment = readFileIntoString("shaders/fragment.glsl");
	}
	else {
		this->m_sVertex = this->m_engine->getMenu()->getVertexEditor()->GetText();
		this->m_sFragment = this->m_engine->getMenu()->getFragmentEditor()->GetText();
	}

	this->m_iShaderProgram = this->compileShaderProgram(this->m_sVertex.c_str(), this->m_sFragment.c_str());
}


GLuint Render::compileShader(GLenum type, const char* szCode) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &szCode, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader Compilation Failed: " << infoLog << std::endl; // todo
	}

	return shader;
}

GLuint Render::compileShaderProgram(const char* szVertexShader, const char* szFragmentShader) {
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, szVertexShader);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, szFragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader Program Linking Failed: " << infoLog << std::endl; // todo
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}




int Render::getFullscreenMode() {
	return this->m_fullscreen_mode;
}

void Render::setFullscreenMode(int mode) {
	this->m_fullscreen_mode = mode;
	SDL_SetWindowFullscreen(this->getSDLWindow(), this->m_fullscreen_mode);
}

int* Render::getScreenDimensions() {
	int* dims = new int[2];
	dims[0] = this->m_screen_w;
	dims[1] = this->m_screen_h;
	return dims;
}

SDL_Window* Render::getSDLWindow() {
	return this->m_window;
}

SDL_GLContext Render::getSDLGLContext() {
	return this->m_gl_context;
}

std::string Render::getVertexShaderCode() {
	return this->m_sVertex;
}

std::string Render::getFragmentShaderCode() {
	return this->m_sFragment;
}