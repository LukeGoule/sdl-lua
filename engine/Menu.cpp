#include <SDL.h>
#include <sstream>

#include "Menu.hpp"
#include "Engine.hpp"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_sdl2.h"
#include "../ImGui/imgui_impl_opengl3.h"
#include "../ImGuiColorTextEdit/TextEditor.h"

#include "glm/gtc/type_ptr.hpp"

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

	//ImGui::SetNextWindowSize(ImVec2(350, 200));
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

		if (ImGui::TreeNode("Show unsmoothed values")) {
			for (const auto fBand : PORTAUDIO()->getFrequencyBands())
			{
				std::stringstream val;
				val << fBand->m_name << " ";
				val << fBand->m_fCurrentAmp;

				ImGui::PlotLines(fBand->m_name.c_str(), Funcs::Get, fBand->m_pAmpHistory, PORTAUDIO_HISTORY_LENGTH, 0, val.str().c_str(), 0.f, 6.f, ImVec2(-1.f, 50.f));
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Show smoothed values")) {
			for (const auto fBand : PORTAUDIO()->getFrequencyBands())
			{
				std::stringstream val;
				val << fBand->m_name << " ";
				val << fBand->m_fCurrentAmpSmoothed;

				ImGui::PlotLines(fBand->m_name.c_str(), Funcs::Get, fBand->m_pAmpHistorySmooth, PORTAUDIO_HISTORY_LENGTH, 0, val.str().c_str(), 0.f, 6.f, ImVec2(-1.f, 50.f));
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Select a device"))
		{
			static int selected = 0;
			int n = 0;
			for (const auto pDevice : PORTAUDIO()->getDevices())
			{
				bool isSelected = selected == n;
				
				if (ImGui::Selectable(pDevice->m_name.c_str(), &isSelected)) {
					selected = n;
					PORTAUDIO()->selectDevice(n);
				}

				n++;
			}

			ImGui::TreePop();
		}

		ImGui::SliderFloat("Smoothing", &OPTIONS()->get()->m_fSmoothFactor, 0.01f, 1.0f, "%.3f");
		
		if (ImGui::Button("Load devices")) {
			PORTAUDIO()->enumerateDevices();
		}

		if (ImGui::Button("Clear data")) {
			PORTAUDIO()->clearHistories();
		}

		if (ImGui::Button("Compile")) {
			RENDER()->recompileShaders();
		}
	}
	ImGui::End();

	//ImGui::ShowDemoWindow();

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

	ImGui::SetNextWindowSize(ImVec2(500, 350));
	ImGui::Begin("Options");
	{
		ImGui::DragFloat("Zoom", &OPTIONS()->get()->m_fZoom, 10.f, 200.f, 20000000.f);
		ImGui::DragFloat2("Center", glm::value_ptr(OPTIONS()->get()->m_vecCenter), 1/OPTIONS()->get()->m_fZoom, -100.f, 100.f);
		ImGui::DragInt("Iterations", &OPTIONS()->get()->m_iIterations, 1, 10, 100000000);

		ImGui::Spacing();
		ImGui::DragFloat3("View Position", &OPTIONS()->get()->m_vecViewpos.x, 1.f, -100.f, 100.f);
		ImGui::DragFloat("View Rotation", &OPTIONS()->get()->m_vecViewrot.x, 1.f, -180.f, 180.f);
		ImGui::DragFloat3("Light Position", &OPTIONS()->get()->m_lightPosition.x, .1f, -5.f, 5.f);
		ImGui::DragFloat3("Light Colour", &OPTIONS()->get()->m_lightColour.x, .1f, -5.f, 5.f);
		ImGui::DragFloat("FOV", &OPTIONS()->get()->m_fFOV, 1.f, 45.f, 120.f);
	}
	ImGui::End();


	this->m_engine->getHooks()->callHooks("render_imgui");

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