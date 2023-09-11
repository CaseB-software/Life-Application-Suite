#include "Settings.h"


Settings::Settings() : Module("Settings") {

}
Settings::~Settings(){
    
}

bool Settings::init(){



    return true;
}

void Settings::run() {
	static bool cmd{ false };

	static bool file{ false };
	if (ImGui::Begin("Settings", &m_shown, 0)) {
		ImGui::Text("Main path:  \t\t%s",	Application::getInstance().printParentDirectory().c_str());

		ImGui::Separator();

		ImGui::NewLine();
		ImGui::Text("As of v0.0.1, this currently is not working");
		ImGui::Checkbox("CMD", &cmd); ImGui::SameLine();
		ImGui::Checkbox("File", &file);

	}
	ImGui::End();
}