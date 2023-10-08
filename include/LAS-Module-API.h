#ifndef MODULE_H
#define MODULE_H


#include "DearImGUI/imgui.h"

#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>


#ifdef LAS_EXPORTING
#define LAS_API __declspec(dllexport)
#endif
#ifndef LAS_EXPORTING
#define LAS_API __declspec(dllimport)
#endif

#ifdef _WIN64

    #include <Windows.h>

#endif

namespace LAS {
    class IChildWindow {
    public:
        IChildWindow(const std::string title = "New Child Window", const ImVec2 minWinSize = ImVec2(200, 200), const ImVec2 maxWinSize = ImVec2(500, 500));
        virtual ~IChildWindow() = 0;

        std::string m_title;

        const ImVec2 m_maxWinSize, m_minWinSize;
        ImVec2 m_curWinSize;

    private:
    };


    class IModule {
    public:
        IModule(const std::string setName = "New Module", const std::string& parentDirectory = "");
        virtual ~IModule() = 0;

        std::string printName()         const { return m_name; }
        std::string printDirectory()    const { return m_directory.string(); }

        bool setDirectory(const std::string& parentPath);

        virtual bool performFirstTimeSetup() = 0;
        virtual bool init()                  = 0;
        virtual void run()                   = 0;
        virtual void shutdown()              = 0;

        bool m_shown{ false };

    protected:
        std::string m_name;
        std::string m_moduleTag;


    private:
        std::filesystem::path m_directory{};

        //  std::unordered_map<std::string, ChildWindow> m_childWindows;

    };


    // Exported Functions
    extern "C" {
        LAS_API IModule* getModule();
    }

}






#endif