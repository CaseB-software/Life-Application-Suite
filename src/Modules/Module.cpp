#include "Module.h"


Module::Module(const std::string setName)  : m_name{setName} {
}  

Module::~Module(){
    
}

void Module::run(){
    if(ImGui::Begin("Default Name", &m_shown, 0)){
        ImGui::Text("There is no overriden funciton specified for %s", m_name.c_str());
    }
    ImGui::End(); //End Window
}

bool Module::init(){
    return false;
}

bool Module::doesDirectoryExist(std::string parentDirectory) {

    Utilities::ensureBackslash(parentDirectory);

    m_directory =  parentDirectory + m_name + "\\";

    return FileSystem::doesFileExist(m_directory.string());
}

bool Module::performFirstTimeSetup() {
    return false;
}


// NEED A WAY FOR APPLICATION TO KEEP TRACK OF MODULES