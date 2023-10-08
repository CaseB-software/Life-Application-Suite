#pragma once

#include "LAS-Module-API.h"

class Settings : public LAS::IModule 
{
public:
	Settings();
	~Settings();

	bool performFirstTimeSetup() = 0;
	bool init()		override;
	void run()		override;
	void shutdown()	override;

private:
};

