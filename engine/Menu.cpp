#include <SDL.h>

#include "Menu.hpp"
#include "Engine.hpp"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl2.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGuiColorTextEdit/TextEditor.h"

#include "portaudio.h"

Menu::Menu(Engine* engine) : EngineModule(engine) {}

Menu::~Menu() {}

void Menu::init() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();

	this->m_pVertexEditor = new TextEditor;
	this->m_pFragmentEditor = new TextEditor;

	this->m_pVertexEditor->SetLanguageDefinition(TextEditor::LanguageDefinitionId::Glsl);
	this->m_pFragmentEditor->SetLanguageDefinition(TextEditor::LanguageDefinitionId::Glsl);

	this->m_pVertexEditor->SetText(this->m_engine->getRender()->getVertexShaderCode());
	this->m_pFragmentEditor->SetText(this->m_engine->getRender()->getFragmentShaderCode());
}

void Menu::initOpenGL() {
	ImGui_ImplSDL2_InitForOpenGL(
		this->m_engine->getRender()->getSDLWindow(), 
		this->m_engine->getRender()->getSDLGLContext()
	);

	ImGui_ImplOpenGL3_Init();
}

void Menu::render() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(350, 200));
	ImGui::Begin("App");
	{
		double audioLoad = Pa_GetStreamCpuLoad(this->m_engine->getPortaudio()->getPaStream());
		ImGui::Text("%.3f ms/frame -- %.1f FPS", 1000.0f / io.Framerate, io.Framerate);
		ImGui::Text("Portaudio CPU %%: %.2f", audioLoad * 100.f);

		const auto pEngine = &Engine::getInstance();

		struct Funcs {
			static float Get(void* data, int i) {
				return ((float*)data)[i] * 100.f;
			}
		};

		ImGui::PlotLines("Low", Funcs::Get, pEngine->getPortaudio()->getLowHistory(), 70, 0, NULL, 0.f, 1.f);
		ImGui::PlotLines("Mid", Funcs::Get, pEngine->getPortaudio()->getMidHistory(), 70, 0, NULL, 0.f, 1.f);
		ImGui::PlotLines("High", Funcs::Get, pEngine->getPortaudio()->getHighHistory(), 70, 0, NULL, 0.f, 1.f);

		if (ImGui::Button("Compile")) {
			this->m_engine->getRender()->recompileShaders();
		}

		;
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(500, 350));
	ImGui::Begin("Vertex Shader");
	{
		this->m_pVertexEditor->Render("Vertex Code");
	}
	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(500, 350));
	ImGui::Begin("Fragment Shader");
	{
		this->m_pFragmentEditor->Render("Fragment Code");
	}
	ImGui::End();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Menu::handleSDLEvent(SDL_Event* pEvent) {
	ImGui_ImplSDL2_ProcessEvent(pEvent);
}

TextEditor* Menu::getVertexEditor() {
	return this->m_pVertexEditor;
}

TextEditor* Menu::getFragmentEditor() {
	return this->m_pFragmentEditor;
}