#include "ModuleInterface.h"


ModuleInterface::ModuleInterface() {

}
ModuleInterface::~ModuleInterface() {
	delete m_module;
}

ModuleInterface::ModuleInterface(ModuleInterface&& other) {
	m_module = std::move(other.m_module);
	m_dllHandle = other.m_dllHandle;
	
}
ModuleInterface& ModuleInterface::operator=(ModuleInterface&& old) noexcept {
	if (this != &old) {
		m_module = std::move(old.m_module);
		m_dllHandle = old.m_dllHandle;

		return *this;
	}
}


bool ModuleInterface::loadDLL(const std::wstring& path) {
	if (std::filesystem::exists(path)) {
		m_dllHandle = LoadLibrary(path.c_str());
		return (m_dllHandle);
	}
	else
		return false;
}
bool ModuleInterface::loadModule() {
	typedef LAS::Module* (*getModuleType)();
	getModuleType getModule;
	getModule = (getModuleType)GetProcAddress(m_dllHandle, "getModule");
	m_module = getModule();

	return m_module;
}
LAS::Module& ModuleInterface::getModule() {
	return *m_module;
}