#pragma once

#include "LAS-Module-API.h"
#include "Utilities/UTL_Files.h"

#include <memory>


class ModuleInterface
{
public:
	ModuleInterface();
	~ModuleInterface();

	ModuleInterface(ModuleInterface&& other);
	ModuleInterface& operator=(ModuleInterface&& old) noexcept;

	bool loadDLL(const std::wstring& path);
	bool loadModule();

	LAS::Module& getModule();

private:
	HINSTANCE m_dllHandle;

	LAS::Module* m_module{nullptr};


};

