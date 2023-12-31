#ifndef APPLICATION_H
#define APPLICATION_H

// User-created include
#include "Utilities/FileSystem.h"
#include "OpenLog.h"

#include "Log Target/LogManager.h"
#include "Modules/Home.h"

#include "Modules/Module.h"
#include "Modules/DebugLog.h"
#include "Modules/Vehicle.h"

// Standard library include
#include <vector>
#include <Windows.h>
#include <iostream> // Specifically for first time setup. Not needed in every header include


// 3rd Party Library include
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_impl_glfw.h>
#include <DearImGUI/imgui_impl_opengl3.h>


class Application final
{
public:
	static Application& getInstance() noexcept;
	~Application();
	
	void init();		// Goes through all Setup member functions
	void run();			// Contians main loop


	// Get functions
	std::string printParentDirectory()	const	{ return PARENT_DIRECTORY.string(); }
	std::string printModuleDirectory()	const	{ return MODULE_DIRECTORY.string(); }
	std::string printLogDirectory()		const	{ return LOG_DIRECTORY.string(); }
	std::string printWindowTitle()		const	{ return WINDOW_TITLE; 				}

	void	registerModule	(Module& module);
	Module* getModule		(const std::string name) const;
	std::vector<std::string> getAllModuleNames() const;



	// Window variables
	const ImVec2 	MIN_WIN_SIZE	{1280, 720};
	GLFWwindow* 	m_window 		{nullptr};
	int 			m_window_x		{1280};
	int 			m_window_y		{720};
	ImGuiStyle* 	appStyle		{nullptr};


private:
	Application();
	const Application& operator=(const Application& app) = delete;

	std::unordered_map<std::string, std::unique_ptr<Module>> m_registeredModules;							// Holds all Modules
	LogManager m_LM					{ "LOG MANAGER" };
	
	const std::string LAS_TAG		{ "LAS" };						// Used for marking logs originating within LAS functions
	const std::string VERSION		{ "v0.3.0-WIP" };
	const std::string WINDOW_TITLE	{ "Life Application Suite" };

	bool m_initialized	{ false };
	bool m_vsync 		{ true };
	

	// Paths necessary to run. The interaction and variables
	std::string getExeParentPath() const;						// Returns the parent directory of the EXE path
	void 		AssignPaths(const std::string parentPath);		// Assigns member variable paths given the parentPath
	
	std::filesystem::path 	PARENT_DIRECTORY;						// Main parent directory that houses the executable
	std::filesystem::path	MODULE_DIRECTORY;
	std::filesystem::path	LOG_DIRECTORY;


	// Setup functions. Does setup in the order listed here
	bool SetupFileSystem();										// Set member path varialbes and initializes RST
	bool SetupGLFW();											
	bool SetupImGUI();											// Creates window flags and sets colors
	bool SetupModules();										// Iterates through s_moduleList and runs the Setup() function
	


};

std::string FirstTimeSetup(std::string oldDirectory); 					// Called if SetupFileSystem() could not find directories necessary, thereby assuming FirstTimeSetup



#endif