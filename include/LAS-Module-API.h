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

    class IModule {
    public:
        IModule(const std::string& setName = "New Module");
        virtual ~IModule();

        std::string printName() const;

        virtual bool init()      = 0;
        virtual void run()       = 0;
        virtual void shutdown()  = 0;

        bool m_shown{ false };

    protected:
        std::string m_name;

    private:

    };


    // Exported Functions
    extern "C" {
        LAS_API IModule* getModule();
    }

}






#endif