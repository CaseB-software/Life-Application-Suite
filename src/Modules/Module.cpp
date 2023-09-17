#include "Module.h"


Module::Module(const std::string setName)  : m_name{setName} {
    m_moduleTag = utl::txt::toUpper(m_name);
    OpenLog::Tag tag{ m_moduleTag };
    OpenLog::registerTag(tag);
}  

Module::~Module(){
    
}

void Module::run(){
    if(ImGui::Begin(m_name.c_str(), &m_shown, 0)) {
        ImGui::Text("There is no overriden funciton specified for %s", m_name.c_str());
    }
    ImGui::End(); //End Window
}

bool Module::init(){
    return false;
}

bool Module::doesDirectoryExist(std::string parentDirectory) {

    utl::txt::ensureBackslash(parentDirectory);

    m_directory =  parentDirectory + m_name + "\\";

    return utl::fs::doesFileExist(m_directory.string());
}

bool Module::performFirstTimeSetup() {
    return false;
}

bool Module::setDirectory(const std::string& path) {
    if (!utl::fs::createDirectory(path)) {
        return false;
    }
    else {
        std::string direcBuf{ path };
        utl::txt::ensureBackslash(direcBuf);
        m_directory = direcBuf + m_name + "\\";
        return true;
    }
}



// NEED A WAY FOR APPLICATION TO KEEP TRACK OF MODULES