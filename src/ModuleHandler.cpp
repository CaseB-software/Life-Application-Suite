#include "ModuleHandler.h"


IModuleDLL::IModuleDLL(const std::wstring& path) {
	load(path);
}
IModuleDLL::IModuleDLL() {

}
IModuleDLL::~IModuleDLL() {

}

int IModuleDLL::load(const std::wstring& path) {
	if (std::filesystem::exists(path)) {
		m_libHandle = LoadLibrary(path.c_str());
		

		// Check if DLL successfully loaded
		if (m_libHandle){ 

			// Load from LAS API exported function getModule()
			typedef LAS::IModule* (*getModuleType)();
			getModuleType getModule;
			getModule = (getModuleType)GetProcAddress(m_libHandle, "getModule");

			m_module = std::make_unique<LAS::IModule>(getModule());

			if (m_module)	{ return 0; }	// IModule loaded properly
			else			{ return 3; }	// Could not load IModule

			return 0;
		}
		else { 
			return 2; 	// Could not load DLL
		}
	}
	else
		return 1;		// Could not find file
}