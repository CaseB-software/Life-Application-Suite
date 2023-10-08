#pragma once

#include "LAS-Module-API.h"
#include "Utilities/UTL_Files.h"

#include <memory>


class IModuleDLL
{
public:
	IModuleDLL();
	~IModuleDLL();

	IModuleDLL(IModuleDLL&& other);
	IModuleDLL& operator=(IModuleDLL&& old) noexcept;

	bool loadDLL(const std::wstring& path);
	bool loadModule();

	LAS::IModule& getModule();

private:
	HINSTANCE m_dllHandle;

	LAS::IModule* m_module{nullptr};


};

