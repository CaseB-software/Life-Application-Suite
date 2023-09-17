#ifndef MODULE_H
#define MODULE_H

#include "Utilities/UTL_Files.h"
#include "Utilities/UTL_Text.h"
#include "OpenLog/OpenLog.h"


#include <string>
#include <vector>
#include <iostream>

#include <DearImGUI/imgui.h>
#include <DearImGUI/imgui_mods.h>



class Module{
public:
    Module(const std::string setName="No Name");
    virtual ~Module();

    const std::string printName()  const   { return m_name;    }
    const bool        isShown()    const   { return m_shown;   }

    bool setDirectory(const std::string& parentPath);
    std::string printDirectory()        const  { return m_directory.string(); }

    bool doesDirectoryExist(std::string parentDirectory);

    virtual bool performFirstTimeSetup();
    virtual bool init();    
    virtual void run();

    bool m_shown{ false };    // Whether or not this module is currently opened in ImGui


protected:
    std::string m_name;             // Defines the name of the module to be used
    std::string m_moduleTag;

    std::filesystem::path m_directory{};

private:
   
};



class ChildWindow{
public:
    ChildWindow(const std::string title="No Name", const ImVec2 minWinSize=ImVec2(200,200), const ImVec2 maxWinSize=ImVec2(500,500));
    ~ChildWindow();


    const ImVec2 maxWinSize, minWinSize;
    ImVec2 currentWinSize;

private:
};


#endif