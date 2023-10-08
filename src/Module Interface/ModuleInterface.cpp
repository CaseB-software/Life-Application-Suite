#include "ModuleInterface.h"


IModuleDLL::IModuleDLL() {

}
IModuleDLL::~IModuleDLL() {
	delete m_module;
}

IModuleDLL::IModuleDLL(IModuleDLL&& other) {
	m_module = std::move(other.m_module);
	m_dllHandle = other.m_dllHandle;
	
}
IModuleDLL& IModuleDLL::operator=(IModuleDLL&& old) noexcept {
	if (this != &old) {
		m_module = std::move(old.m_module);
		m_dllHandle = old.m_dllHandle;

		return *this;
	}
}


bool IModuleDLL::loadDLL(const std::wstring& path) {
	if (std::filesystem::exists(path)) {
		m_dllHandle = LoadLibrary(path.c_str());
		return (m_dllHandle);
	}
	else
		return false;
}
bool IModuleDLL::loadModule() {
	typedef LAS::IModule* (*getModuleType)();
	getModuleType getModule;
	getModule = (getModuleType)GetProcAddress(m_dllHandle, "getModule");
	m_module = getModule();

	return m_module;
}
LAS::IModule& IModuleDLL::getModule() {
	return *m_module;
}