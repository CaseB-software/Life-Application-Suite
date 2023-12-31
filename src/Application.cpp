#include "Application.h"

Application& Application::getInstance() noexcept {
	static Application app;
	return app;
}
Application::Application() {

}
Application::~Application() {

}

// Needed for GLFW Setup
static void glfw_error_callback(int error, const char* description)
{
	// Log the error
	std::ostringstream logText;
	logText << "GLFW Error " << error << ": " << description;
	std::cout << logText.str() << std::endl;
}

void Application::init() {

	OpenLog::Tag las{ LAS_TAG };
	OpenLog::registerTag(las);
	OpenLog::changeSettings(OpenLog::TIME_OFFSET, -7);

	if (!m_initialized) {
		// Ensure all proper subsystems are initialized
		if (SetupFileSystem()) {

			m_LM.setParentDirectory(LOG_DIRECTORY.string());
			if (m_LM.createLogFile()) {
				OpenLog::registerLogTarget(std::make_unique<LogManager>(m_LM));
				OpenLog::addActiveLogTarget(m_LM.str());
				
			}

			OpenLog::log("Application file system was setup successfully", LAS_TAG);


			if (SetupGLFW()) {
				OpenLog::log("GLFW was setup successfully", LAS_TAG);
				if (SetupImGUI()) {
					OpenLog::log("ImGUI was setup successfully", LAS_TAG);
					if (SetupModules()) {
						OpenLog::log("All Modules are setup", LAS_TAG);
						m_initialized = true;
						OpenLog::log("Application was successfully setup", LAS_TAG);
					}
				}
			}
		}
	}	
	
	// Exit if every subsystem could noe be initialized
	if(!m_initialized){
		throw std::exception("Application initialization failed.");
		return;
	}
}
void Application::run()
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); //for testing

	while (!glfwWindowShouldClose(this->m_window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// My code goes here for window calls ------------
		Home();
		// -----------------------------------------------

		// Rendering
		ImGui::Render();
		glfwGetFramebufferSize(this->m_window, &this->m_window_x, &this->m_window_y);
		glViewport(0, 0, this->m_window_x, this->m_window_y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		glfwSwapBuffers(this->m_window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->m_window);
	glfwTerminate();

	return;
}

void Application::registerModule(Module& module) { 
	m_registeredModules.try_emplace(module.printName(), std::make_unique<Module>(module)); 
}
Module* Application::getModule(const std::string name) const {
	if (m_registeredModules.contains(name)) {
		return m_registeredModules.at(name).get();
	}
	else
		return nullptr;
}

std::vector<std::string> Application::getAllModuleNames() const{
	std::vector<std::string> returnBuf;

	for (auto& module : m_registeredModules) {
		returnBuf.push_back(module.second->printName());
	}

	return returnBuf;
}


//-----END PUBLIC-------


//-----PRIVATE-----
bool Application::SetupFileSystem(){
	bool success{false};

	AssignPaths(getExeParentPath());
	
	// Checks if member variable directories exist yet. If they do not exist, start first time setup
	if (!FileSystem::doesFileExist(printModuleDirectory()))
	{
		AssignPaths(FirstTimeSetup(printParentDirectory()));
	}		
	

	// Creates directories for the files if the files did exist/first time setup complete
	if (!FileSystem::createDirectory(printParentDirectory()) || !FileSystem::createDirectory(printLogDirectory()))
	{
		OpenLog::log("Could not initialize the file system.", LAS_TAG);
		success = false;
	}
	else 
	{
		success = true;
	}

	return success;
}

bool Application::SetupGLFW(){
	bool success {false};

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()){
		throw std::exception{ "GLFW could not be initialized" };
	}
	else{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

		m_window = glfwCreateWindow(m_window_x, m_window_y, WINDOW_TITLE.c_str(), NULL, NULL);
		if(!m_window){
			throw std::exception{ "GLFW could not create window" };
		}
		else {
			glfwMakeContextCurrent(m_window);
			glfwSetWindowSizeLimits(m_window, MIN_WIN_SIZE.x, MIN_WIN_SIZE.y, GLFW_DONT_CARE, GLFW_DONT_CARE);
			gladLoadGL(); 
			glfwSwapInterval(m_vsync); // Enable vsync
			success = true;
		}
	}
	return success;
}
bool Application::SetupImGUI(){
	const char* glsl_version = "#version 460";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     	// Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     	// Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.FontGlobalScale = (1.3f); 
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
				

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	appStyle = &ImGui::GetStyle();
	if (io.ConfigFlags)
	{
		appStyle->WindowRounding = 0.0f;
		appStyle->WindowBorderSize = 1.0f;
		appStyle->FramePadding = ImVec2(8, 4);

		appStyle->Colors[ImGuiCol_MenuBarBg] 		= ImColor(0, 162, 237, 255);
		appStyle->Colors[ImGuiCol_HeaderHovered] 	= ImColor(30, 30, 30, 255);

		appStyle->Colors[ImGuiCol_TitleBg] 			= ImColor(115, 115, 115, 255);
		appStyle->Colors[ImGuiCol_TitleBgActive] 	= ImColor(115, 115, 115, 255);
		appStyle->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(115, 115, 115, 135);

		appStyle->Colors[ImGuiCol_TabUnfocused] 		= ImColor(60, 60, 60, 255);
		appStyle->Colors[ImGuiCol_TabUnfocusedActive] 	= ImColor(60, 60, 60, 255);
		appStyle->Colors[ImGuiCol_TabHovered] 			= ImColor(60, 60, 60, 135);
		appStyle->Colors[ImGuiCol_TabActive] 			= ImColor(30, 30, 30, 255);
		appStyle->Colors[ImGuiCol_Tab] 					= ImColor(60, 60, 60, 255);
		appStyle->TabRounding = 0.0f;
	}

	return true;
}
bool Application::SetupModules(){

	Debug 			debugManager	{};
	VehicleManager 	vehicleManager 	{};

	// The order of calling AddModule() will determine the order they are in the menu bar
	registerModule(vehicleManager);
	registerModule(debugManager);

	// If there are no modules, exit
	if(m_registeredModules.empty())
	{
		std::cerr << "There are no Modules for this application" << std::endl;
		return false;
	}

	// Iterate through the module list and run the setup function
	for(auto& module : m_registeredModules){
		if(!module.second->init()){
			std::string msg{"Could not setup [" + module.second->printName() + "] and did not add to Module list"};
			OpenLog::log(msg, LAS_TAG);
		}
		else { 
			OpenLog::log( "Successfully initialized Module " + module.second->printName(), LAS_TAG);
		}
	}

	return true;
}



std::string Application::getExeParentPath() const {

	std::filesystem::path pathBuffer{};

#ifdef _WIN32
	std::cerr << "Windows OS was detected" << std::endl;
	
	#ifdef _DEBUG
		pathBuffer = "C:\\dev\\1. Project Development Files\\LAS\\";
		return pathBuffer.string();
	#endif

	#ifndef _DEBUG
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);			// Windows specific call to return parent directory of the EXE
		pathBuffer = buffer;								// Gets the parent directory path
		return pathBuffer.parent_path().string() + "\\";	// return the directory name followed by a backslash
		/*
		*  This is for when I get my developer license and can write to Program Files
		*
			// If the OS is x64 bit windows, set to normal program files
			#ifdef _WIN64
				pathBuffer 	= "C:\\Program Files\\LAS\\";
			#endif

			// If the OS is x32 bit windows, set to x86 program files
			#ifndef _WIN64
				pathBuffer = "C:\\Program Files (x86)\\LAS\\";
			#endif
		*/
	#endif

#endif


#ifndef _WIN32
	std::cerr << "Windows OS was not detected. This version of LAS does not support other operating systems. Exiting Application" << std::endl;
	throw (true);
#endif

	return pathBuffer.string();
}
void Application::AssignPaths(const std::string parentPath){

	// Assign member variables but does NOT create the directory
	PARENT_DIRECTORY	= parentPath;
	MODULE_DIRECTORY	= PARENT_DIRECTORY.string() + "Modules\\";
	LOG_DIRECTORY		= PARENT_DIRECTORY.string() + "Logs\\";

	OpenLog::log("Set LAS parent directory to " + printParentDirectory(), LAS_TAG);
	
	return;
}

std::string FirstTimeSetup(std::string oldDirectory) {
	std::cerr << "Necessary directories were not found. Performing first time setup." << std::endl;

	std::string desiredDirectoryBuffer{};

	bool inputAccepted{ false };
	do {
		bool confirmDesiredDirectoryAccepted{ false };
		char desiredDirectoryInput{};


		system("cls");
		char input{};
		std::cout << "Required file directories were not found in " << oldDirectory << "\nWould you like to use this directory to store the application's files? (y/n)\n";
		std::cin >> input;

		switch (input) {
		case 'y': case 'Y':
			inputAccepted = true;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			desiredDirectoryBuffer = oldDirectory;
			// Do nothing thereby using the already assignged folder paths of where the exe is
			break;
		case 'n': case 'N':
			std::cin.clear();
			std::cin.ignore(1000, '\n');

			// Enter desired file path AND confirm it loop
			do {
				system("cls");
				std::cout << "Enter desired parent directory for files: ";
				std::cout << "\n  Note: As of v0.2.0-WIP, the .exe needs to be in the same parent directory.\n  The folder containing the .exe will also contain the files\n>";
				std::getline(std::cin, desiredDirectoryBuffer);

				Utilities::ensureBackslash(desiredDirectoryBuffer);

				std::cout << "\nDesired directory >\n" << desiredDirectoryBuffer << "\n\n";
				std::cout << "Are you sure this is correct? (y/n)\n";
				std::cin >> desiredDirectoryInput;

				switch (desiredDirectoryInput) {
				case 'y': case 'Y':
					confirmDesiredDirectoryAccepted = true;
					inputAccepted = true;

					std::cin.clear();
					std::cin.ignore(1000, '\n');
					break;
				case 'n': case 'N':
					confirmDesiredDirectoryAccepted = false;
					std::cin.clear();
					std::cin.ignore(1000, '\n');
					break;
				default:
					std::cout << "\nInvalid input. Restarting...";
					confirmDesiredDirectoryAccepted = false;
					Sleep(2000);
					break;
				}
			} while (!confirmDesiredDirectoryAccepted);

			break;
		default:
			std::cout << "\nInvalid input. Restarting...";
			inputAccepted = false;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			Sleep(2000);
			break;
		}
	} while (!inputAccepted);

	system("cls");

	return desiredDirectoryBuffer;
}



