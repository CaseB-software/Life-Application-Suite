#pragma once
#include <unordered_map>
#include <memory>

#include "Utilities/UTL_Files.h"
#include "LAS-Module-API.h"

class IModuleDLL
{
public:
	IModuleDLL(const std::wstring& path);
	IModuleDLL();
	~IModuleDLL();

	int load(const std::wstring& path);					// Return 0 if all worked well, 1 if could not find file, 2 if could not load dll,  3 if could not load IModule

	LAS::IModule& getModule();

private:
	HINSTANCE m_libHandle{};

	std::unique_ptr<LAS::IModule> m_module;
};



class ModuleManager
{
public:
	ModuleManager(std::string_view directory);
	~ModuleManager();

	bool registerModule		(IModuleDLL* module);		// Moves parameter to m_modules
	bool removeModule		(std::string_view key);		// Returns true if removed, false if could not be removed

private:
	std::unordered_map<std::string, std::unique_ptr<LAS::IModule>> m_modules;
	std::filesystem::path m_moduleDirectory{};
};

