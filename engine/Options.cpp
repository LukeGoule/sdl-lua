#include "Options.hpp"
#include "Engine.hpp"

Options::Options(Engine* instance) : EngineModule(instance) {
	this->m_pOptions = new Options::Options_t;
}

Options::~Options() {

}

void Options::init() {
	// TODO: Load configuration files into m_pOptions
}

Options::Options_t* Options::get() {
	return this->m_pOptions;
}