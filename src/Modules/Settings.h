#ifndef SETTINGS_H
#define SETTINGS_H

#include "Module.h"
#include "../Application.h"
#include <DearImGUI/imgui.h>

// View, edit, and save settings for the entire application
// AS OF 6 NOV 2022, Can only view file paths used in the application


class Settings : public Module {
public:
    Settings();
    ~Settings() override;

    void run()  override;
    bool init()    override;
};

#endif