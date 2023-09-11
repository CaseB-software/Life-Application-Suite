#include "Home.h"


void Home()
{
	// Intent to remove in release ---------
	static bool showDemoWindow 	= false;
	// -------------------------------------
	
	// Create Docking Window over full viewport----------------------------------------
	static ImGuiIO& io = ImGui::GetIO();
	if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
		static const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoScrollbar;


		if(ImGui::Begin("Main Dockspace", nullptr, window_flags)) {
			static ImGuiID dockspace_id;
			dockspace_id = ImGui::GetID("MainDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

			// The difference in the call to BeginMenuBar() here comapred to the else statement's BeginMainMenuBar() is
			// that the dockspace occludes the MainMenuBar since the dockspace is itself a window
			if(ImGui::BeginMenuBar()){
				MenuBar(showDemoWindow);
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	} // -----------------------------------------------------------
	

	// No Docking else statement
	else {
		if(ImGui::BeginMainMenuBar()){
			MenuBar(showDemoWindow);
			ImGui::EndMainMenuBar();
		}
	}
	

	// Show all modules in the module list and actually draw the window
	for(std::string name : Application::getInstance().getAllModuleNames()){
		Module* module{ Application::getInstance().getModule(name) };
		if(module->isShown()){
			module->run();
		}
	}
	
	// Intent to remove in release ---------
	if(showDemoWindow) 			{ ImGui::ShowDemoWindow(); }
	// -------------------------------------

	return;
}

void MenuBar(bool &demoWindow){
	// Creates Dropdown item in the menu bar labeled "Modules"
	if(ImGui::BeginMenu("Modules")){
		// Iterate through module list and display the option
		for(auto& name : Application::getInstance().getAllModuleNames()){
			ImGui::MenuItem(name.c_str(), nullptr, Application::getInstance().getModule(name)->m_shown);
		}

		ImGui::EndMenu();
	}

	// Creates Dropdown item in the menu bar labeled "View"
	if(ImGui::BeginMenu("View")){
		ImGui::MenuItem("Demo Window", nullptr, &demoWindow);
		ImGui::EndMenu();
	}
}


