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
	this->m_engine->getHooks()->callHooks("load_resources");

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

	this->m_engine->getHooks()->callHooks("init_opengl");

	constexpr float W = 1.f;

	float vertices[] = {
		// Positions         // Colors
		 W,  W, 0.0f,  1.0f, 0.0f, 0.0f,  // Top Right
		 W, -W, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom Right
		-W, -W, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom Left
		-W,  W, 0.0f,  1.0f, 1.0f, 0.0f   // Top Left 
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint VBO, VAO, EBO;
	this->m_iShaderProgram = this->m_engine->getShaders()->compileShaderProgram(this->m_sVertex.c_str(), this->m_sFragment.c_str());
	
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

	float u_zoom = 400.f;

	// Setup transformations here
	glm::highp_mat4 model		= glm::mat4(1.0f);
	glm::highp_mat4 view		= glm::mat4(1.0f);
	glm::highp_mat4 projection	= glm::highp_mat4();

	UNIFORMS()
		->addUniform(Uniforms::UniformType::FLOAT1, &OPTIONS()->get()->m_fZoom, "u_zoom")
		->addUniform(Uniforms::UniformType::FLOAT2, &OPTIONS()->get()->m_vecCenter, "u_center")
		->addUniform(Uniforms::UniformType::INT1, &OPTIONS()->get()->m_iIterations, "u_iterations");

	UNIFORMS()
		->addUniform(Uniforms::UniformType::MAT4, glm::value_ptr(model), "m_mat4Model")
		->addUniform(Uniforms::UniformType::MAT4, glm::value_ptr(view), "m_mat4View")
		->addUniform(Uniforms::UniformType::MAT4, glm::value_ptr(projection), "m_mat4Projection");

	UNIFORMS()
		->addUniform(Uniforms::UniformType::FLOAT1, PORTAUDIO()->getPLow(), "m_fLowAmplitude")
		->addUniform(Uniforms::UniformType::FLOAT1, PORTAUDIO()->getPMid(), "m_fMidAmplitude")
		->addUniform(Uniforms::UniformType::FLOAT1, PORTAUDIO()->getPHigh(), "m_fHighAmplitude");

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

		projection = glm::perspective(glm::radians(OPTIONS()->get()->m_fFOV), (float)this->m_screen_w / (float)this->m_screen_h, 0.1f, 100.0f);

		glViewport(0, 0, this->m_screen_w, this->m_screen_h);
		glOrtho(0, this->m_screen_w, this->m_screen_h, 0, -1, 1);

		float angleX = glm::radians(OPTIONS()->get()->m_vecViewrot.x),
			angleY = glm::radians(OPTIONS()->get()->m_vecViewrot.y),
			angleZ = glm::radians(OPTIONS()->get()->m_vecViewrot.z);

		model = glm::rotate<float>(glm::mat4(1.f), angleX, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate<float>(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate<float>(model, angleZ, glm::vec3(0.0f, 1.0f, 0.0f));

		view = glm::translate(glm::mat4(1.0f), OPTIONS()->get()->m_vecViewpos);

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		glUseProgram(this->m_iShaderProgram);

		GLuint timeLoc = glGetUniformLocation(this->m_iShaderProgram, "m_iTime");
		GLuint resolutionLoc = glGetUniformLocation(this->m_iShaderProgram, "m_vec2Resolution");
		GLuint mousePosLoc = glGetUniformLocation(this->m_iShaderProgram, "m_vec2MousePosition");
		
		glUniform1f(timeLoc, currentTime);
		glUniform2f(resolutionLoc, (float)this->m_screen_w, (float)this->m_screen_h);
		glUniform2f(mousePosLoc, (float)mx, (float)my);

		this->m_engine->getUniforms()->setOpenGLUniforms(this->m_iShaderProgram);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		this->m_engine->getHooks()->callHooks("render");

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
		this->m_engine->getHooks()->callHooks("post_render");
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

	this->m_iShaderProgram = this->m_engine->getShaders()->compileShaderProgram(this->m_sVertex.c_str(), this->m_sFragment.c_str());
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