#include "DebugLog.h"


Debug::Debug() : Module("Debug Menu"){
	m_logs = new std::vector<std::string>;
}

Debug::~Debug(){
	delete m_logs;
}


bool Debug::init(){
	//RST::SetLogVector(m_logs);

	return true;
}


void Debug::run(){
	 if(ImGui::Begin(m_name.c_str(), &m_shown, 0)){

        static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);
		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

		static bool AutoScroll { true };
		static bool ScrollToBottom { true };

		ImGuiMods::Header("Debug Logs", "Shows all Debugs Logged for this instance");
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::SameLine();

		if(AutoScroll){
			 ImGui::SetScrollHereY(1.0f);
		}


		if(ImGui::Button("Add Log", ImVec2(100, TEXT_BASE_HEIGHT))){
			static int addLogCounter{0};
			++addLogCounter;
			std::ostringstream os; os << "Test Log " << addLogCounter;
			std::cerr << os.str() << std::endl;
		}


		ImGui::Spacing();

		// Log Region
		 if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), true, ImGuiWindowFlags_HorizontalScrollbar))
        {	

			// Write the string to the window
			for(std::string& s : *m_logs){
				ImGui::TextUnformatted(s.c_str());
			}

			if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;

			
		}
		ImGui::EndChild();	 

		const std::chrono::system_clock::time_point now { std::chrono::system_clock::now()};

		ImGui::TextUnformatted(std::format("{:%H:%M:%OS}", now).c_str());


		ImGui::End(); //End Window
	 }


	return;
}




