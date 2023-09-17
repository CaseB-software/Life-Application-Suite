#include "Vehicle.h"

std::vector<Vehicle> VehicleManager::s_vehicleList{}; // Initializer for s_vehicleList to be used

#pragma warning(disable : 4996) // For use of sprintf

Application& app{ Application::getInstance() };


// Operator Overloads
std::ostream& 	operator<<	(std::ostream& os, const Repair& repair) {
	os << 		repair.m_mileageDone 
	<< '|' << 	repair.m_type 
	<< '|' << 	repair.m_cost 
	<< '|' << 	repair.m_isThirdPartyRepair 
	<< '|' << 	repair.m_notes 
	<< '|' << 	repair.m_date.getRawTime();

	return os;
}
std::ostream& 	operator<<	(std::ostream& os, const GasStop& gasStop) {
	os << 		gasStop.m_mileageDone 
	<< '|' << 	gasStop.m_pricePerGallon 
	<< '|' << 	gasStop.m_gallons 
	<< '|' << 	gasStop.m_notes 
	<< '|' << 	gasStop.m_date.getRawTime();

	return os;
}
std::ostream& 	operator<<	(std::ostream& os, const RepairType& type){
	switch(type){
		case RepairType::BATTERY_REPLACEMENT:
			os << "Battery Replacement";
			break;
		case RepairType::BODYWORK:
			os << "Bodywork";
			break;
		case RepairType::LIGHTBULB_REPLACEMENT:
			os << "Lightbulb Replacement";
			break;
		case RepairType::MECHANICAL:
			os << "Mechanical";
			break;
		case RepairType::OIL_CHANGE:
			os << "Oil Change";
			break;
		case RepairType::OTHER:
			os << "Other";
			break;
		case RepairType::POWER_STEERING_FLUID_EXCHANGE:
			os << "Power Steering";
			break;
		case RepairType::TIRE_REPLACEMENT:
			os << "Tire Replacement";
			break;
		case RepairType::TIRE_ROTATION:
			os << "Tire Rotation";
			break;
		case RepairType::TRANSMISSION_FLUID_EXCHANGE:
			os << "Transmission Fluid";
			break;
		case RepairType::WIPER_REPLACEMENT:
			os << "Wiper Blade Replacement";
			break;
		default:
			os << "No Repair Type Found";
			break;
	}

	return os;
}
bool 			operator==	(const Vehicle& lhs, const Vehicle& rhs){
	if(lhs.getName() == rhs.getName()){
		return true;
	}
	else{
		return false;
	}
}



void Repair::getRepairInfo(int& mileage, std::string& typeStr, double& costDbl, std::string& notesVar, bool& wasThirdParty, std::string& dateVar) const {
	mileage 		= 	m_mileageDone;
	costDbl 		= 	m_cost;
	notesVar 		= 	m_notes;
	wasThirdParty 	= 	m_isThirdPartyRepair;
	dateVar 		= 	m_date.printDate();

	std::ostringstream typeVarBuf; 		// Makes a string stream bufferr to insert m_type
	typeVarBuf << m_type;				// (using he overloaded operator<< to write 
	typeStr = typeVarBuf.str();			// the type in readable form) and then overwrites the parameter with the string
	
	return;
}
void GasStop::getGasStopInfo(int& mileage, double& gal, double& ppg, std::string& notesVar, std::string& dateVar) const{
	mileage 	= m_mileageDone;
	gal 		= m_gallons;
	ppg 		= m_pricePerGallon;
	notesVar	= m_notes;
	dateVar 	= m_date.printDate();
	return;
}



Vehicle::Vehicle(const std::string name, uint32_t mileage) {
	
	setName(name);
	// Make sure the mileage is not less than 0
	if (mileage < 0) {
		std::cerr << "Vehicle mileage for [" + m_name + "] was below zero. Set to 0" << std::endl;
		m_mileage = 0;
	}
	else {
		m_mileage = mileage;
	}


	return;
}
bool Vehicle::NewRepair(uint32_t setMiles, RepairType setType, double setCost, std::string setNotes, bool setThirdParty, utl::time::Timestamp date) {

	// State variables used for error checking
	bool milesAccepted{ false }, costAccepted{ false }, notesAccepted{ false };

	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setCost >= 0) 	{ costAccepted 	= true; }

	notesAccepted 	= CheckStringSize(setNotes, maxNotesSize);						// Ensure the notes size is not longer than maxNotesSize

	// If all conditions are valid, make the Repair and add it to the Vehicle's list of repairs
	if (milesAccepted && costAccepted && notesAccepted) {
		Repair repBuf{ setMiles, setType, setCost, setNotes, setThirdParty, date };
		repairList.push_back(repBuf);

		// Sorts the Vehicle's repair list.
		// Check if sorting is needed (not needed if repBuf is higher than the last repairList element)
		if (repairList.size() == 1 || repBuf.getMileage() > repairList[repairList.size() - 2].getMileage()) { } // Does nothing if no need to sort
		else { 
			// Sort the Vehicle's repair list by mileage
			for (int i{ 0 }; i < repairList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < repairList.size(); ++x) {
					if (repairList[x].getMileage() < repairList[i].getMileage()) {
						std::swap(repairList[i], repairList[x]);
					}
				}
			}
		}

		std::cerr << "Added Repair to [" + getName() +']' << std::endl;
		setMileage(setMiles);
		setLasUpdated(date);

		return true;
	}
	else {
		std::cerr << "Could not add Repair to [" + getName() +']' << std::endl;
		if(!milesAccepted) 	{ std::cerr << "Mileage was not accepted" << std::endl; }
		if(!costAccepted) 	{ std::cerr << "Cost was not accepted" << std::endl; }
		if(!notesAccepted) 	{ std::cerr << "Notes were not accepted" << std::endl; }
		return false;
	}

}
bool Vehicle::NewGasStop(uint32_t setMiles, double setGal, double setPPG, std::string setNotes, utl::time::Timestamp date) {
	// Stae variables
	bool milesAccepted{ false }, galAccepted{ false }, ppgAccepted{ false }, notesAccepted{ false };

	if (setMiles > 0) 	{ milesAccepted = true; }
	if (setGal > 0) 	{ galAccepted 	= true; }
	if (setPPG >= 0) 	{ ppgAccepted = true; }

	// Ensure the notes size is not longer than maxNotesSize
	notesAccepted 	= CheckStringSize(setNotes, maxNotesSize);

	// If all inputs are valid, make GasStop type and add to list
	if (milesAccepted && galAccepted && ppgAccepted && notesAccepted) {
		GasStop gsBuffer{ setMiles, setGal, setPPG, setNotes, date };
		gasList.push_back(gsBuffer);

		// Sort by mileage
		// If the New GasStop's mileage is higher than the last element in the vector, no need to sort.
		if (gasList.size() == 1 || gsBuffer.getMileage() > gasList[gasList.size() - 2].getMileage()) { } // Does nothing if no need to sort
		else { 
			for (int i{ 0 }; i < gasList.size() - 1; ++i) {
				for (int x{ i + 1 }; x < gasList.size(); ++x) {
					if (gasList[x].getMileage() < gasList[i].getMileage()) {
						std::swap(gasList[i], gasList[x]);
					}
				}
			}
		}

		std::cerr << "Added Gas Stop to [" + getName() +']' << std::endl;
		setMileage(setMiles);
		setLasUpdated(date);

		return true;
	}
	else {
		std::cerr << "Could not add Gas Stop to [" + getName() +']' << std::endl;
		if(!milesAccepted) 	{ std::cerr << "Mileage was not accepted" << std::endl; }
		if(!galAccepted) 	{ std::cerr << "Number of Gallons was not accepted" << std::endl; }
		if(!ppgAccepted) 	{ std::cerr << "Price Per Gallon was not accepted" << std::endl; }
		if(!notesAccepted) 	{ std::cerr << "Notes were not accepted" << std::endl; }

		return false;
	}
}

bool Vehicle::setMileage(const uint32_t setMileage){
	bool success{false};

	// If given mileage is greater, assign it to the vehicle
	if (setMileage > m_mileage) {
		m_mileage = setMileage;
		success = true;

		// Log mileage was changed
		std::ostringstream txt{}; txt <<"Set Vehicle [" << getName() <<"] mileage to [" << m_mileage << "]";
		std::cerr << txt.str() << std::endl;
	}
	else{
		std::ostringstream txt{}; txt <<"Mileage was not set for Vehicle [" << getName() <<"]. Current mileage is [" << m_mileage << "], given mileage was [" << setMileage << "]";
		std::cerr << txt.str() << std::endl;
	}

	return success;
}
bool Vehicle::setLasUpdated(const utl::time::Timestamp date){
	bool success{false};

	if(date.getRawYMD().year() < m_lastUpdated.getRawYMD().year()) {
		std::cerr << "Given year was prior to Last Updated year for [" + getName() + "]" << std::endl;
		return false;
	}
	else{
		if(date.getRawYMD().month() < m_lastUpdated.getRawYMD().month()){
			std::cerr << "Given month was prior to Last Updated month for [" + getName() + "]" << std::endl;
			return false;
		}
		else{
			if(date.getRawYMD().month() == m_lastUpdated.getRawYMD().month() && date.getRawYMD().day() < m_lastUpdated.getRawYMD().day()){
				std::cerr << "Given day was prior to Last Updated day for [" + getName() + "]" << std::endl;
				return false;
			}
			else{
				m_lastUpdated = date;
				std::ostringstream txt; txt << m_lastUpdated.printDate();
				std::cerr << "Set Vehicle [" + getName() + "] last updated to [" + txt.str() + "]" << std::endl;
				success = true;
			}
		}
	}

	return success;
}
bool Vehicle::setName(const std::string setName){
	bool success{false};

	// Make sure the name is not longer than maxVehicleNameSize
	if (!CheckStringSize(setName, maxVehicleNameSize)) {
		m_name = "Vehicle";

		// Log the error
		std::ostringstream logText;
		logText << "Vehicle name [" << setName << "] was too long. The name was set to \"Vehicle\" ";
		std::cerr << logText.str() << std::endl;
		
		success = false;
	}
	else {
		std::cerr << "Vehicle [" + m_name + "] has been renamed to [" + setName + "]" << std::endl;
		m_name = setName;
		
		success = true;
	}

	return success;
}



Vehicle* VehicleManager::SelectableVehicleList(bool &reset) {
	static Vehicle* selVeh{nullptr};

	if(reset){ reset = false; selVeh = nullptr; } // If reset is true, make selected vehicle null and put the flag back to false

	// Create the selectable list of vehicles
	if(s_vehicleList.size() > 0){
		if (ImGui::BeginListBox("Current Vehicles", ImVec2(ImGui::GetWindowSize().x, 120))) 
		{
			// Numbers the vehicles in the list
			static int selected = -1;
			int vehNum{0};
			for (Vehicle& currentVehicle : s_vehicleList )
			{	
				++vehNum;
				char buf[32];
				sprintf(buf, "%d. %s", vehNum, currentVehicle.getName().c_str());
				if (ImGui::Selectable(buf, selected == vehNum))
				{
					selected = vehNum;
					selVeh = &currentVehicle;
				}
				ImGui::SameLine(200); ImGui::Text("%d miles", currentVehicle.getMileage()); 
			}
			ImGui::EndListBox();
		}
	}
	else{ ImGui::Text("There are no tracked vehicles"); }

	return selVeh;
}
void VehicleManager::ShowFullVehicleInformation(Vehicle* veh) {
	static const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * 8);

	ImGui::Text("Repairs");
	ImGui::Spacing();
	if(ImGui::BeginTable("Repairs", 6, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
	{
		ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Cost", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Third Party", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableHeadersRow();

		for (Repair& rep : veh->getRepairList())
		{
			ImGui::TableNextRow();

			// Buffers for the specified repair that will be overweitten
			int				mileBuf;
			std::string		typeBuf;
			double			costBuf;
			std::string		notesBuf;
			bool			thirdPartyBuf;
			std::string		dateBuf;

			// Write to the buffers
			rep.getRepairInfo(mileBuf, typeBuf, costBuf, notesBuf, thirdPartyBuf, dateBuf);

			// Display in the table
			int column{0};
			ImGui::TableSetColumnIndex(column);	
			ImGui::Text("%s", dateBuf.c_str());						// Date
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);								// Mileage
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", typeBuf.c_str());						// Type of Repair
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("$ %4f", costBuf);							// Cost of Repair
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", thirdPartyBuf ? "true" : "false");	// Third Party
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());					// Notes
		}
	ImGui::EndTable();
	}

	ImGui::NewLine();
	ImGui::Text("Gas Stops");
	ImGui::Spacing();

	if(ImGui::BeginTable("Gas Stops", 5, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg , outer_size ))
	{
		ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Miles", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Gallons", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Per Gal", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableSetupColumn("Notes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
		ImGui::TableHeadersRow();

		for (GasStop& gas : veh->getGasStopList())
		{
			ImGui::TableNextRow();

			// Buffers for the specified GasStop that will be overweitten
			int			mileBuf;
			double		galBuf;
			double		costBuf;
			std::string notesBuf;
			std::string dateBuf;

			// Write to the buffers
			gas.getGasStopInfo(mileBuf, galBuf, costBuf, notesBuf, dateBuf);

			int column{0};

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", dateBuf.c_str());		// Date
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%d", mileBuf);				// Mileage
			++column;

			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%f", galBuf);				// Gallons total
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("$ %f", costBuf);			    // Cost per gallon
			++column;
			
			ImGui::TableSetColumnIndex(column);
			ImGui::Text("%s", notesBuf.c_str());	// Notes
		}
	ImGui::EndTable();
	}
	return;
}
bool VehicleManager::SaveAllVehicles	() {
	bool success{ false };

	// Iterates through s_vehicleList and writes information to a file of it's m_name
	for (Vehicle& currentVehicle : s_vehicleList) {
		if (!SaveVehicle(&currentVehicle)) {
			std::cerr << "Could not save vehicle information for " + currentVehicle.getName() << std::endl;
			success = false;
		}
		else {
			std::cerr << "Saved vehicle information for " + currentVehicle.getName() << std::endl;
			success = true;
		}
	}
	return success;
}

// -----------------------




// Helper Functions

std::string MakeVehicleName		(std::string& text){
	//Check if the text string exists. If not, exit function
	if (text == "") { return ""; }

	std::string vehNameBuf{};

	// State Variables
	bool writeChar{ false }, exitLoop{ false };
	int charactersRead{ 0 };

	//Reads text between ( ) to write to vehNameBuf
	for (const char& c : text) { 
		++charactersRead;
		if (c == ')') 	{ writeChar = false; exitLoop = true; }
		if (writeChar) 	{ vehNameBuf += c; }
		if (c == '(') 	{ writeChar = true; }

		// Deletes the number of read characters from the text string
		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			return vehNameBuf; 
		}
	}

	return vehNameBuf;
}
uint32_t 	MakeVehicleMiles	(std::string& text){
	//Check if the text string exists. If not return 0 miles
	if (text == "") { return 0; }

	std::stringstream mileStringBuf{};
	uint32_t mileBuf{0};

	// Staate variables
	bool writeChar{ false }, exitLoop{ false };

	int charactersRead{ 0 };
	for (const char& c : text) { //Reads text between ( ) to write to vehNameBuf
		++charactersRead;
		if (c == '}') 	{ writeChar = false; exitLoop = true; }
		if (writeChar) 	{ mileStringBuf << c; }
		if (c == '{') 	{ writeChar = true; }
		
		// Deletes read characters
		if (exitLoop) {
			for (int i{ 0 }; i < charactersRead; ++i) {
				text.erase(0, 1);
			}
			mileStringBuf >> mileBuf;
			return mileBuf;
		}

	}
	return mileBuf;
}
void 		MakeRepair			(std::string& text, Vehicle& veh){
	std::string repBuf;							//Buffer of characters being read from text
	std::vector<std::string> repairStrings; 	//Vector of strings of repair info not yet formatted

	//Reads between < > to find raw repair strings and adds to list
	bool writeChar{ false };
	for (char& c : text) {
		if (c == '>') {
			repairStrings.push_back(repBuf); 	// Adds the string to list of unformatted repairs
			repBuf.erase();						// Resets the buffer to write again
			writeChar = false;
		}

		if (writeChar) 	{ repBuf += c; }
		if (c == '<') 	{ writeChar = true; }
	}


	// For logging
	size_t 	numRepairsTotal{repairStrings.size()};
	unsigned short 	numRepSuccessfullyAddedToVehicle{0}, numRebFailedToAddToVehicle{0};

	//Reads unformatted repairStrings, formats them into Repair types and adds to vehicle
	char separator{ '|' };
	for (std::string currentString : repairStrings) {

		// Buffer variables
		uint32_t				mileBuf{};
		std::ostringstream		typeBuf{};
		double					costBuf{};
		std::ostringstream		notesBuf{};
		bool					thirdPartyBuf{};
		std::stringstream		dateStringBuf{};


		// Parsing the text to the buffers. Needs to be in the order of however the file is written
		// in order to write to the correct data types
		utl::txt::readUntilAuto(currentString, separator, mileBuf);
		typeBuf << utl::txt::readUntilString(currentString, separator);
		utl::txt::readUntilAuto(currentString, separator, costBuf);
		utl::txt::readUntilAuto(currentString, separator, thirdPartyBuf);
		notesBuf << utl::txt::readUntilString(currentString, separator);
		dateStringBuf << utl::txt::readUntilString(currentString, separator);


		// Make the typeBuf raw string into a RepairType enum for use
		// in creating a Repair. 
		std::string typeBufString{typeBuf.str()};
		RepairType enumTypeBuf;
		{
			if(typeBufString == "Battery Replacement"){
				enumTypeBuf = RepairType::BATTERY_REPLACEMENT;
			}
			else if(typeBufString == "Lightbulb Replacement"){
				enumTypeBuf = RepairType::LIGHTBULB_REPLACEMENT;
			}
			else if(typeBufString == "Bodywork"){
				enumTypeBuf = RepairType::BODYWORK;
			}
			else if(typeBufString == "Mechanical"){
				enumTypeBuf = RepairType::MECHANICAL;
			}
			else if(typeBufString == "Oil Change"){
				enumTypeBuf = RepairType::OIL_CHANGE;
			}
			else if(typeBufString == "Other"){
				enumTypeBuf = RepairType::OTHER;
			}
			else if(typeBufString == "Power Steering"){
				enumTypeBuf = RepairType::POWER_STEERING_FLUID_EXCHANGE;
			}
			else if(typeBufString == "Tire Replacement"){
				enumTypeBuf = RepairType::TIRE_REPLACEMENT;
			}
			else if(typeBufString == "Tire Rotation"){
				enumTypeBuf = RepairType::TIRE_ROTATION;
			}
			else if(typeBufString == "Transmission Fluid"){
				enumTypeBuf = RepairType::TRANSMISSION_FLUID_EXCHANGE;
			}
			else if(typeBufString == "Wiper Blade Replacement"){
				enumTypeBuf = RepairType::WIPER_REPLACEMENT;
			}
			else{
				enumTypeBuf = RepairType::OTHER;
			}
		}

		// Read the dateStringBuf string and make a Timestamp type
		utl::time::Timestamp dateBuf{};
		dateBuf.stamp(utl::time::stringToTimepoint(dateStringBuf.str()));

		// Create the repair type
		if(veh.NewRepair(mileBuf, enumTypeBuf, costBuf, notesBuf.str(), thirdPartyBuf, dateBuf)) { ++numRepSuccessfullyAddedToVehicle; }
		else { ++numRebFailedToAddToVehicle; }
	}

	// Log total unformatted strings, how many were successful at adding to the vehicle and how many failed
	std::ostringstream msg; 
	msg << "Vehicle file [" << veh.getName() <<"] had [" << numRepairsTotal << "] unformatted Repair strings. [" 
		<< numRepSuccessfullyAddedToVehicle << "] were added to vehicle, [" << numRebFailedToAddToVehicle << "] could not be added to vehicle";
	std::cerr << msg.str() << std::endl;

	return;
}
void 		MakeGasStop			(std::string& text, Vehicle& veh){

	std::string foundGasBuf;				//Buffer of characters being read from text
	std::vector<std::string> gasStrings;	//Vector of unformatted GasStop strings

	//Read between [ ], write to gasStrings to be formatted to GasStops
	bool writeChar{ false };
	for (char& c : text) {
		if (c == ']') {
			gasStrings.push_back(foundGasBuf);
			foundGasBuf.erase();
			writeChar = false;
		}
		if (writeChar) 	{ foundGasBuf += c; }
		if (c == '[') 	{ writeChar = true; }
	}

	// For logging
	size_t numGSTotal{gasStrings.size()};
	unsigned short 	numGSSuccessfullyAddedToVehicle{0}, numGSFailedToAddToVehicle{0};


	//Reads unformatted gasStrings, formats them into GasStop types and adds to Vehicle
	char separator{ '|' };
	for (std::string currentString : gasStrings) {
		// Buffer Variables
		uint32_t			mileBuf{};
		double				gallonsBuf{};
		double				ppgBuf{};
		std::ostringstream	notesBuf{};
		std::stringstream	dateStringBuf{};

		// Parsing the text to the buffers. Needs to be in the order of however the file is written
		// in order to write to the correct data types
		utl::txt::readUntilAuto(currentString, separator, mileBuf);
		utl::txt::readUntilAuto(currentString, separator, ppgBuf);
		utl::txt::readUntilAuto(currentString, separator, gallonsBuf);
		notesBuf << utl::txt::readUntilString(currentString, separator);
		dateStringBuf << utl::txt::readUntilString(currentString, separator);

		// Read the dateStringBuf string and make a Timestamp type
		utl::time::Timestamp dateBuf{};
		dateBuf.stamp(utl::time::stringToTimepoint(dateStringBuf.str()));

		if(veh.NewGasStop(mileBuf, gallonsBuf, ppgBuf, notesBuf.str(), dateBuf)){ ++numGSSuccessfullyAddedToVehicle; }
		else { ++numGSFailedToAddToVehicle; }
	}

	// Log total unformatted strings, how many were successful at adding to the vehicle and how many failed
	std::ostringstream msg; 
	msg << "Vehicle file [" << veh.getName() <<"] has [" << numGSTotal << "] unformatted Gas Stop strings. [" 
		<< numGSSuccessfullyAddedToVehicle << "] were added to vehicle, [" << numGSFailedToAddToVehicle << "] could not be added to vehicle";
	std::cerr <<  msg.str() << std::endl;

	return;
}
bool 		CheckStringSize		(const std::string text, int maxAllowed){
	// If length is zero, it's below the max allowed so return TRUE
	if(text.length() == 0){
		return true;
	}

	// Ensure the notes size is not longer than maxNotesSize
	if (text.length() < 0 || text.length() > maxAllowed) {
		std::ostringstream logText;
		logText << "String [" << text << "] out of range at [" << text.length() << "] characters. Max allowed is [" << maxAllowed << "]";
		std::cerr << logText.str() << std::endl;

		return false;
	}
	else { return true; }
}

bool 		AddGasStop(Vehicle* veh, bool& wasSaved){
	bool success{false};

	// Flags for if input was not accepted, these set to TRUE
	static bool showMileageOutOfRangeMsg { false };
	static bool showGalOutOfRangeMsg	 { false };
	static bool showPPGOutOfRangeMsg	 { false };
	static bool badNotes	{false};

	// Input field buffers
	static uint32_t 	mileBuf		{ 0 };
	static double 		galBuf		{ 0.0 };
	static double 		ppgBuf		{ 0.0 };
	static char 		notesBuf	[veh->maxNotesSize];

	// Check inputs as they are typed
	if(mileBuf >= 500000 || mileBuf <= 0)   { mileBuf = 0; showMileageOutOfRangeMsg = true;}	else {showMileageOutOfRangeMsg 	= false; }
	if(galBuf >= 300 || galBuf <= 0) 		{ galBuf = 0; showGalOutOfRangeMsg 		= true;} 	else {showGalOutOfRangeMsg 		= false; }
	if(ppgBuf >= 300 || ppgBuf < 0) 		{ ppgBuf = 0; showPPGOutOfRangeMsg 		= true;} 	else {showPPGOutOfRangeMsg 		= false; }


	ImGuiMods::CenterText("Enter in the required information to add a new Gas Stop to the Vehicle");
	ImGui::NewLine();

	ImGui::Text("Mileage          "); ImGui::SameLine();
 	ImGui::InputScalar("##MileBuf", ImGuiDataType_U32, &mileBuf, NULL, NULL, "%u");
	if(showMileageOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Mileage out of range. Mileage must be real.");}
	

	ImGui::Text("Gallons          "); ImGui::SameLine();
 	ImGui::InputScalar("##galBuf", ImGuiDataType_Double, &galBuf, NULL, NULL, "%lf");
	if(showGalOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Gallons out of range. Gallons must be real.");}

	ImGui::Text("Price Per Gallon "); ImGui::SameLine();
 	ImGui::InputScalar("##ppgBuf", ImGuiDataType_Double, &ppgBuf, NULL, NULL, "%lf");
	if(showPPGOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Price Per Gallon out of range. Price Per Gallon must be real.");}


	ImGui::Text("Notes            "); ImGui::SameLine();
	ImGui::InputText("##Notes", notesBuf, veh->maxNotesSize);
	if(badNotes) {ImGui::SameLine(); ImGui::Text("Notes out of range. Max size allowed is %d characters", Vehicle::maxVehicleNameSize); }


	static bool inputsAccepted { false };
	static bool alreadySaved{ false };
	if(ImGuiMods::BeginPopupModal("New Gas Stop")){
	
		if(inputsAccepted){
			// Inputs were accepted
			if (!alreadySaved) {
				utl::time::Timestamp now; now.stamp();
				success = veh->NewGasStop(mileBuf, galBuf, ppgBuf, std::string{ notesBuf }, now); alreadySaved = true; 
			}

			// Reset Buffers
			mileBuf = 0;
			ppgBuf 	= 0.0;
			galBuf 	= 0.0;
			
			int i{0};						// Clear all of the notesBuffer
			for( char& c : notesBuf) { notesBuf[i] = NULL; ++i; }
					
			wasSaved = true;

			ImGui::NewLine(); ImGui::Text("Gas Stop added to %s", + veh->getName().c_str()); ImGui::NewLine();
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); alreadySaved = false; inputsAccepted = false;}

		}
		else{
			ImGuiMods::CenterText("One or more of your inputs were invalid. \nPlease try again");
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup();  alreadySaved = false; }
		}
		ImGui::EndPopup();
	}
	

	// Assign the buffers to the vehicle
	ImGui::NewLine();
	ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - 75 );
	if(ImGui::Button("Save", ImVec2(150, 30))){
		// Moved checking of inputs here to remove the repeated checking in the Popup Modal loop
		if(CheckStringSize(notesBuf, Vehicle::maxVehicleNameSize)) 	{ badNotes = false; } else { badNotes = true; }

		if(mileBuf > 0 && ppgBuf >= 0.0 && galBuf > 0.0 && !badNotes) { inputsAccepted = true; } else { inputsAccepted = false; }

		ImGui::OpenPopup("New Gas Stop"); 
	}

	return success;
}
bool 		AddRepair(Vehicle* veh, bool& wasSaved){
	bool success{false};

	// Flags for if input was not accepted, these set to TRUE
	static bool showMileageOutOfRangeMsg { false };
	static bool showCostOutOfRangeMsg	 { false };
	static bool badNotes	{false};

	// Input field buffers
	static uint32_t 	mileBuf			{ 0 };
	static double 		costBuf			{ 0.0 };
	static bool 		thirdPartyBuf;
	static char 		notesBuf		[Vehicle::maxVehicleNameSize];

	// Check inputs as they are typed
	if(mileBuf >= 500000 || mileBuf <= 0) { mileBuf = 0; showMileageOutOfRangeMsg = true;}	else {showMileageOutOfRangeMsg = false; }
	if(costBuf >= 500000 || costBuf < 0) { costBuf = 0; showCostOutOfRangeMsg = true;} 		else {showCostOutOfRangeMsg = false; }

	ImGuiMods::CenterText("Enter in the required information to add a new Repair to the Vehicle");
	ImGui::NewLine();

	ImGui::Text("Mileage         "); ImGui::SameLine();
 	ImGui::InputScalar("##MileBuf", ImGuiDataType_U32, &mileBuf, NULL, NULL, "%u");
	if(showMileageOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Mileage out of range. Mileage must be real.");}


	ImGui::Text("Type	        "); ImGui::SameLine();
	const char* repairTypes[] = { "Oil Change", "Transmission Fluid Exchange", "Lightbulb Replacement", "Power Steering Fluid Exchange",
		 "Wiper Blade Replacement", "Tire Rotation", "Tire Replacement", "Bodywork", "Mechanical Work", "Battery Replacement", "Other" }; 	// Options for the combo box
	
	static int repairTypeIndex = 10; 																										// Here we store our selected member data as an index of the array
	const char* combo_preview_value = repairTypes[repairTypeIndex];  																		// Pass in the preview value visible before opening the combo (it could be anything)
	if (ImGui::BeginCombo("##RepairType", combo_preview_value))
	{
		for (int n = 0; n < IM_ARRAYSIZE(repairTypes); n++)
		{
			const bool is_selected = (repairTypeIndex == n);
			if (ImGui::Selectable(repairTypes[n], is_selected))
				repairTypeIndex = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected) { ImGui::SetItemDefaultFocus(); }
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Cost	        "); ImGui::SameLine();
 	ImGui::InputScalar("##cost", ImGuiDataType_Double, &costBuf, NULL, NULL, "%lf");
	if(showCostOutOfRangeMsg) {ImGui::SameLine(); ImGui::Text("Cost out of range. Cost must be real."); }


	ImGui::Text("Notes           "); ImGui::SameLine();
	ImGui::InputText("##Notes", notesBuf, veh->maxNotesSize);
	if(badNotes) {ImGui::SameLine(); ImGui::Text("Notes out of range. Max size allowed is %d characters", Vehicle::maxVehicleNameSize); }

	ImGui::Checkbox("Third Party", &thirdPartyBuf);


	static bool inputsAccepted { false };
	static bool alreadySaved{ false };
	if(ImGuiMods::BeginPopupModal("New Repair")){
	
		if(inputsAccepted){
			// Inputs were accepted
			utl::time::Timestamp now; now.stamp();
			if(!alreadySaved) { success = veh->NewRepair(mileBuf, static_cast<RepairType>(repairTypeIndex + 1), costBuf, std::string{notesBuf}, thirdPartyBuf, now); alreadySaved = true;} 

			// Reset Buffers
			mileBuf = 0;
			costBuf = 0.0;
			repairTypeIndex = 10;
			
			int i{0};						// Clear all of the notesBuffer
			for( char& c : notesBuf) { notesBuf[i] = NULL; ++i; }
			
			thirdPartyBuf = false;
			
			wasSaved = true;
			
			ImGui::NewLine(); ImGui::Text("Repair added to %s", + veh->getName().c_str()); ImGui::NewLine();
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); alreadySaved = false; inputsAccepted = false;}
		}
		else{
			ImGuiMods::CenterText("One or more of your inputs were invalid. \nPlease try again");
			if(ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup();  alreadySaved = false; }
		}

		ImGui::EndPopup();
	}



	// Assign the buffers to the vehicle
	ImGui::NewLine();
	ImGui::SetCursorPosX( (ImGui::GetWindowWidth() / 2) - 75 );
	if(ImGui::Button("Save", ImVec2(150, 30)))	{ 
		// Moved checking of inputs here to remove the repeated checking in the Popup Modal loop
		if(CheckStringSize(notesBuf, Vehicle::maxVehicleNameSize)) 	{ badNotes = false; } else { badNotes = true; }

		if(mileBuf > 0 && costBuf >= 0.0 && !badNotes) { inputsAccepted = true; } else { inputsAccepted = false; }

		ImGui::OpenPopup("New Repair"); 
	}
	return success;
}


VehicleManager::VehicleManager() : Module("Vehicle Manager") {

}
VehicleManager::~VehicleManager() {

}
bool VehicleManager::performFirstTimeSetup() {
	return utl::fs::createDirectory(m_directory.string());
}
bool VehicleManager::init() {
	// Go through the vehicle folder and store the names of the files
	std::vector<std::string> vehicleFiles;

	if (utl::fs::doesFileExist(m_directory.string())) {
		vehicleFiles = utl::fs::getFilesInDirectory(m_directory.string());
	}
	else {
		if (!performFirstTimeSetup()) {
			return false;
		}
		else {
			return init();
		}
	}
	
	// Iterate through the files in the folder found and create Vehicle types
	for (const std::string& fileName : vehicleFiles) {
		std::ostringstream fileText;

		//Opens file, reads text, outputs to fileText
		if (!utl::fs::readFile(fileName, fileText)) {
			// If vehicle file couldn't be opened to read, do nothing
			// Unsuccessful open logged in readFile()
			std::ostringstream msg;
			msg << "Could not open [" << fileName << "] for reading.";
			OpenLog::log(msg.str(), m_moduleTag);
		}
		else {
			std::string vehInfoBuf{ fileText.str() };	//Stores the file text in string format

			Vehicle vehicleBuffer{ MakeVehicleName(vehInfoBuf), MakeVehicleMiles(vehInfoBuf) };

			MakeRepair(vehInfoBuf, vehicleBuffer);		//Make repairs from the remaining text and add to vehicle
			MakeGasStop(vehInfoBuf, vehicleBuffer);		//Make Gas stop from the remaining text

			addToVehicleList(vehicleBuffer);			//Add vehicleBuffer to the master vehicle list
			
		}
	}

	return true;
}
void VehicleManager::run() {
	if (ImGui::Begin("Vehicle Manager", &m_shown, 0)) {
		m_currentModuleWindowWidth = ImGui::GetWindowSize().x;

		// Forward declaration of Pop-Up Modal
		// Pop-up informing that Information could not be saved
		if (ImGuiMods::BeginPopupModal("Saving Failed")) {
			ImGuiMods::CenterText("Could not save the Vehicle's Information.\nChanges were not made. Please try again!"); ImGui::NewLine(); 	// Message
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 50);
			if (ImGui::Button("Close", ImVec2(100, 30))) { ImGui::CloseCurrentPopup(); m_saveVehInfoFailed = false; }								// Close button
			ImGui::EndPopup();
		}

		// Dispays header
		ImGuiMods::Header("Vehicle Manager", "This Vehicle Manager stores all relevant information about a vehicle such as its name mileage,"
			" and repair and gas stop information. "); ImGui::Spacing(); ImGui::Spacing();


		if (vehListIsEmpty()) {
			ImGui::Text("There are no tracked vehicles");

			ImGui::NewLine();

			ImGui::Spacing();
			ImGui::TextWrapped("A repair can track:");
			ImGui::BulletText("Mileage it was done");
			ImGui::BulletText("The type of repair it was (oil change, light replacement, washer fluid fill, etc.)");
			ImGui::BulletText("How much the total cost was");
			ImGui::BulletText("Was it done by a third party?");
			ImGui::BulletText("Any notes you would like to add");
			ImGui::BulletText("And the date it was done");

			// If the Create Vehicle Window is true, center the CreateVehicle Window and not display the Create button
			if (m_showCreateVehicleWin) { ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - (m_createVehWinHeight / 2)); }
			else {
				// Center button horrizontally and vertically
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 100);
				ImGui::SetCursorPosY(ImGui::GetWindowHeight() / 2 - 25);
				if (ImGui::Button("Create Vehicle", ImVec2(200, 50))) { m_showCreateVehicleWin = true; }
			}
		} // End if Vehicle List is empty
		else {

			// This draws the child window for the vehicle list and resizes
			if (ImGui::GetWindowWidth() > (m_mainWinWidthMax + m_detailedVehWinWidthMax + 20)) {
				m_childWindowSameLine = true;	// If the window is large enough to hold both windows side by size, it does

				// Center the Tracked Vehicle window if it is the only window displayed
				if (!m_showDetailedVehWin) { ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentMainWinWidth); }
			}
			else {
				m_childWindowSameLine = false;	// If the window cannot have windows side by side, default spacing
				ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentMainWinWidth);
			}

			// Draw the Tracked Vehicles Window
			if (ImGuiMods::BeginResizeableChild("Tracked Vehicles", m_mainWinWidthMin, m_mainWinWidthMax, m_mainWinHeight)) {
				m_currentMainWinWidth = ImGui::GetWindowWidth();

				ImGui::SameLine(); ImGuiMods::HelpMarker("Select a vehicle then choose from the options below"); ImGui::Spacing();	// Help marker
				m_selectedVehicle = SelectableVehicleList(m_delVehCalled);														// Show available vehicles list and return with the selected vehicle

				if (m_selectedVehicle) {

					if (ImGui::Button("Create New Vehicle", ImVec2(m_currentMainWinWidth, 30))) { m_showCreateVehicleWin = true; }	// Make the button span the length of the window

					ImGui::NewLine(); ImGuiMods::CenterText("Vehicle Information");		// Header for second portion of window

					// Show number of Repairs and Add Repair button on the same line
					ImGui::Text("Total Repairs:\t   %d", m_selectedVehicle->getRepairList().size()); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_addRepOrGasStopButtonSize.x - m_winPadding);
					if (ImGui::Button("Add Repair", m_addRepOrGasStopButtonSize)) {
						m_showAddRepairWin = true;
						m_showAddGasStopWin = false;
					}

					// Show number of Gas Stops and Add Gas Stop button on same line
					ImGui::Text("Total Gas Stops: \t%d", m_selectedVehicle->getGasStopList().size()); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_addRepOrGasStopButtonSize.x - m_winPadding);
					if (ImGui::Button("Add Gas Stop", m_addRepOrGasStopButtonSize)) { m_showAddGasStopWin = true; m_showAddRepairWin = false; }

					ImGui::NewLine(); ImGui::Text("Last Updated: \t   %s", m_selectedVehicle->getLastUpdated().printDate().c_str());	// Displaying when the vehicle was last updated

					// If changes were made, display message saying changes need to be saved
					if (m_changesNeedToBeSaved) {
						m_mainWinHeight = 450;
						ImGui::NewLine(); ImGui::NewLine(); ImGuiMods::CenterText(std::string{ "Changes have been made to a Vehicle that have not yet been saved." }.c_str());
					}

					// Once/if vehicle is selected, these buttons appear. Display at bottom of screen
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - m_mainWinButtonSize.y - 20);

					// Based on which windows are open, change how many buttons are shown in order to size them appropriately
					if ((m_showDetailedVehWin || m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin) && !m_changesNeedToBeSaved) { m_buttonsShown = 3; }
					else if ((m_showDetailedVehWin || m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin) && m_changesNeedToBeSaved) { m_buttonsShown = 4; }
					else if (!m_showDetailedVehWin && !m_showAddRepairWin && !m_showAddGasStopWin && !m_showCreateVehicleWin && m_changesNeedToBeSaved) { m_buttonsShown = 3; }
					else { m_buttonsShown = 2; }

					// Ensure buttons are at least the min size specified
					if ((m_currentMainWinWidth - m_winPadding) < (m_minButtonSize * m_buttonsShown)) { m_mainWinButtonSize.x = m_minButtonSize; }		// If the size of the main window is too small, set the button size to the minimum
					else { m_mainWinButtonSize.x = (m_currentMainWinWidth - m_winPadding) / m_buttonsShown; }	 											// Math the buttonSize to change with the size of the parent window

					ImGui::Spacing(); 	if (ImGui::Button("Detailed View", m_mainWinButtonSize)) { m_showDetailedVehWin = true; }
					ImGui::SameLine(); 	if (ImGui::Button("Delete Vehicle", m_mainWinButtonSize)) { ImGui::OpenPopup("Delete?"); } // Open Pop-up

					// If changes have been made, show the Save All button
					if (m_changesNeedToBeSaved) { ImGui::SameLine(); if (ImGui::Button("Save All", m_mainWinButtonSize)) { ImGui::OpenPopup("Save All Vehicles?"); } }

					// Pop-up Modal for overwriting all Vehicle Information
					if (ImGuiMods::BeginPopupModal("Save All Vehicles?")) {
						ImGui::Text("This will overwrite ALL Vehicle information.\n\nThis operation cannot be undone!\n\n");							// Message
						if (ImGui::Button("Save", ImVec2(120, 30))) { m_changesNeedToBeSaved = !SaveAllVehicles(); ImGui::CloseCurrentPopup(); }					// Savae Buttom
						ImGui::SetItemDefaultFocus(); ImGui::SameLine(); if (ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }	// Cancel button with default focus
						ImGui::EndPopup();
					}

					// Button to close the detailed info window
					if (m_showDetailedVehWin || m_showAddRepairWin || m_showAddGasStopWin || m_showCreateVehicleWin) {
						ImGui::SameLine();
						if (ImGui::Button("Close All", m_mainWinButtonSize)) { m_showDetailedVehWin = false; m_showAddRepairWin = false; m_showAddGasStopWin = false; m_showCreateVehicleWin = false; }
					}

					if (ImGuiMods::BeginPopupModal("Delete?")) {
						// Change the message of the Popup depending if already failed to delete a Vehicle
						if (!m_delVehFailed) {
							ImGuiMods::CenterText("This will delete all of the Vehicle's information\nThis operation cannot be undone!\n\n"); ImGui::Separator();

							if (ImGui::Button("Delete", ImVec2(120, 0))) {
								Vehicle vehBufferToDel = *m_selectedVehicle; // Assign buffer the selected Vehicle

								m_delVehCalled = true;																			// Throw the deleteVehicleCalled to reset other buffers
								m_selectedVehicle = nullptr;																			// Set selectedVehicle to NULL
								m_showDetailedVehWin = false; m_showAddRepairWin = false; m_showAddGasStopWin = false; m_showCreateVehicleWin = false;	// Close other windows

								// If the vehicle could not be deleted, set flag to true
								if (!DeleteVehicle(vehBufferToDel)) { m_delVehFailed = true; }
								else { m_selectedVehicle = nullptr; }

								ImGui::CloseCurrentPopup();
							}

							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); m_delVehFailed = false; }		// If canceled, revert flags
						}
						else {
							ImGuiMods::CenterText("Failed to Delete Vehicle");
							if (ImGui::Button("Exit", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); m_delVehFailed = false; }
						}
						ImGui::EndPopup();
					} // End popup modal code 
				}
				else {
					// If there is no selected vehicle, display button at bottom of screen
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (m_currentMainWinWidth / 2));		// Center horizontally
					ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);								// Center vertically
					if (ImGui::Button("Create New Vehicle", ImVec2(m_currentMainWinWidth, 30))) { m_showCreateVehicleWin = true; }
				}
				ImGui::EndChild(); // Ending main window viewing tracked vehicles
			} // End Tracked Vehicles Window

			// Make the detailed window view be on the same line if it can hold them both side by side
			if (m_childWindowSameLine) { ImGui::SameLine(); }
			else { ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentDetailedWinWidth); }

			// If there is a selected Vehicle
			if (m_selectedVehicle) {
				if (m_showDetailedVehWin) {
					// Set window sizes based on if the windows are on the same line
					if (!m_childWindowSameLine) { ImGuiMods::BeginResizeableChild("Full Vehicle Information", m_detailedVehWinWidthMin, 1920, 600); }
					else { ImGuiMods::BeginResizeableChild("Full Vehicle Information", m_detailedVehWinWidthMin, m_detailedVehWinWidthMax, 600); }
					m_currentDetailedWinWidth = ImGui::GetWindowWidth();

					ImGui::SameLine(); ImGuiMods::HelpMarker("This shows all the vehicle information"); ImGui::Spacing();	// Help marker

					// Buffers for text boxes to edit vehicle name and mileage
					static uint32_t milesBuf{ 0 };
					static char 	nameBuf[64];

					static bool showMilesageTooBigMsg{ false };

					// See if the selected vehicle has changed, and if so, throw flag to reset values
					static Vehicle* currentVehicle = nullptr;
					if (currentVehicle != m_selectedVehicle) {
						m_resetInputBufDetailedViewWin = true;
						currentVehicle = m_selectedVehicle;
					}

					// Resets the default variables
					if (m_resetInputBufDetailedViewWin) {
						int i{ 0 };
						for (char& c : nameBuf) { nameBuf[i] = NULL; ++i; } 							// Clear all of the nameBuffer
						i = 0;																		// Set index to front of array
						for (const char& c : m_selectedVehicle->getName()) { nameBuf[i] = c; ++i; } 	// Write the Vehicles name to the buffer char by char to display in the text box
						milesBuf = m_selectedVehicle->getMileage();									// Set the buffer equal to the current Vehicle's mileage
						m_resetInputBufDetailedViewWin = false; 								// Once set, remove the flag to allow editing
					}

					// Show Name input box and Save Changes button on the same line
					ImGui::Spacing(); ImGui::Text("Name:  "); ImGui::SameLine();
					ImGui::InputText("##Name", nameBuf, 64); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_mainWinButtonSize.x - m_winPadding);
					if (ImGui::Button("Save Changes", m_mainWinButtonSize)) {
						if (nameBuf != m_selectedVehicle->getName()) { ImGui::OpenPopup("Are You Sure You Want To Save?"); }				// Open Pop-up ensuring you want to change Vehicle's name
						else { if (m_selectedVehicle->setMileage(milesBuf)) { m_changesNeedToBeSaved = true; } } 	// If just mileage is changed, update Vehicle and throw that changes were made
					}

					if (m_saveVehInfoFailed) { ImGui::OpenPopup("Saving Failed"); }

					if (ImGui::BeginPopupModal("Are You Sure You Want To Save?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						if (!CheckStringSize(nameBuf, Vehicle::maxVehicleNameSize)) {
							ImGui::Text("Vehicle name out of range. Max size allowed is %d characters.\nVehicle was not created.", Vehicle::maxVehicleNameSize);
							ImGui::NewLine();
							if (ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						}
						else {
							ImGui::Text("This will overwrite current Vehicle Information to: ");
							ImGui::Text("Name:    %s\nMileage: %d", &nameBuf, milesBuf);
							ImGui::NewLine();

							if (ImGui::Button("Save", ImVec2(120, 30))) {
								std::string oldName{ m_selectedVehicle->getName() };									// Buffer to save current name in case renaming failed to revert back to original
								std::ostringstream oldFileName{ m_directory.string() + oldName + ".dat"};	// Old Vehicle file path
								std::ostringstream newFileName{ m_directory.string() + nameBuf + ".dat"};  	// Renamed target vehicle file path

								// If the Vehicle renaming was not accepted OR file name not valid, revert back to original name
								if (!m_selectedVehicle->setName(nameBuf) || !utl::fs::renameFile(oldFileName.str(), newFileName.str())) {
									ImGui::CloseCurrentPopup();
									m_selectedVehicle->setName(oldName);
									m_saveVehInfoFailed = true;
								}
								else {
									// If vehicle renaming and file rename successful, change mileage, name and save to file
									m_selectedVehicle->setMileage(milesBuf);
									SaveVehicle(m_selectedVehicle);
									ImGui::CloseCurrentPopup();
								}
							}
							ImGui::SetItemDefaultFocus();
							ImGui::SameLine();
							if (ImGui::Button("Cancel", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
						}
						ImGui::EndPopup();
					}

					// Show Mileage text box and clear on the same line
					ImGui::Text("Miles: "); ImGui::SameLine();
					ImGui::InputScalar("##Miles", ImGuiDataType_U32, &milesBuf, nullptr, NULL, "%u"); ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetWindowWidth() - m_mainWinButtonSize.x - m_winPadding);
					if (ImGui::Button("Clear Inputs", m_mainWinButtonSize)) { m_resetInputBufDetailedViewWin = true; }

					if (milesBuf >= 500000) { milesBuf = 0; showMilesageTooBigMsg = true; }
					else { showMilesageTooBigMsg = false; }
					if (showMilesageTooBigMsg) { ImGui::Text("Mileage out of range. Mileage must be real. \nVehicle was not created"); }

					ImGui::NewLine();
					ShowFullVehicleInformation(m_selectedVehicle);	// Display two tables with Repairs and Gas Stops
					ImGui::NewLine();

					// Center close button on the bottom  of window
					ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - (m_addRepOrGasStopButtonSize.x / 2));
					if (ImGui::Button("Close", m_addRepOrGasStopButtonSize)) { m_showDetailedVehWin = false; }

					ImGui::EndChild();
				}
				else { m_resetInputBufDetailedViewWin = true; }
				// End showing detailed vehicle window


				ImGui::NewLine();
				if (m_showAddRepairWin) {
					ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentAddToWinWidth);
					if (ImGuiMods::BeginResizeableChild("Add Repair", m_addToVehicleWidthMin, m_addToVehicleWidthMax, 500)) {
						m_currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddRepair(m_selectedVehicle, m_changesNeedToBeSaved); // Display input fields for adding repair

						if (m_changesNeedToBeSaved) { ImGui::NewLine();	ImGuiMods::CenterText("Changes have been made that need to be saved to the file"); }

						// Center button on bottom of window
						ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - (m_mainWinButtonSize.x / 2));
						ImGui::SetCursorPosY(ImGui::GetWindowHeight() - m_mainWinButtonSize.y - m_winPadding);
						if (ImGui::Button("Close", m_mainWinButtonSize)) { m_showAddRepairWin = false; }

						ImGui::EndChild();
					}
				}

				if (m_showAddGasStopWin) {
					ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentAddToWinWidth);
					if (ImGuiMods::BeginResizeableChild("Add Gas Stop", m_addToVehicleWidthMin, m_addToVehicleWidthMax, 500)) {
						m_currentAddToWinWidth = ImGui::GetWindowSize().x;

						AddGasStop(m_selectedVehicle, m_changesNeedToBeSaved);	// Display input fields for adding Gas Stop

						if (m_changesNeedToBeSaved) { ImGui::NewLine(); ImGuiMods::CenterText("Changes have been made that need to be saved to the file"); }

						ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - (m_mainWinButtonSize.x / 2));
						ImGui::SetCursorPosY(ImGui::GetWindowHeight() - m_mainWinButtonSize.y - m_winPadding);
						if (ImGui::Button("Close", m_mainWinButtonSize)) { m_showAddGasStopWin = false; }

						ImGui::EndChild();
					}
				}
			}
		} // End if vehicle list is not empty

		if (m_showCreateVehicleWin) {
			ImGui::NewLine();
			ImGuiMods::CenterChildWindow(m_currentModuleWindowWidth, m_currentCreateVehWidth);
			ImGuiMods::BeginResizeableChild("Create New Vehicle", m_createVehWidthMin, m_createVehWidthMax, m_createVehWinHeight);
			m_currentCreateVehWidth = ImGui::GetWindowWidth();

			static uint32_t mileBuf{ 0 };
			static char 	nameBuf[64];

			static bool showMilesageTooBigMsg{ false };					// If mileage entered > 500,000, show message saying mileage cannot be that high

			if (m_resetInputBufCreateVehWin) {						// Resets the default variables
				int i{ 0 };
				for (char& c : nameBuf) { nameBuf[i] = NULL; ++i; }			// Clear all of the nameBuffer
				i = 0;
				for (const char& c : "Vehicle Name") { nameBuf[i] = c; ++i; }	// Write the Vehicles name to the buffer to display in the text box
				mileBuf = 0;												// Reset mileage buffer to zero
				m_resetInputBufCreateVehWin = false; 				// Once set, remove the flag to allow editing
			}

			// Show name field and create on same line
			ImGui::NewLine(); ImGui::NewLine();
			ImGui::Text("Name:  "); ImGui::SameLine();
			ImGui::InputText("##Name", nameBuf, 64); ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150 - m_winPadding);
			if (ImGui::Button("Create", ImVec2(150, 30))) {
				ImGui::OpenPopup("Create New Vehicle?");
			}

			// Show mile field and clear on same line
			ImGui::Text("Miles: "); ImGui::SameLine();
			ImGui::InputScalar("##Miles", ImGuiDataType_U32, &mileBuf, nullptr, NULL, "%u");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 150 - m_winPadding);
			if (ImGui::Button("Clear Inputs", ImVec2(150, 30))) { m_resetInputBufCreateVehWin = true; }

			ImGui::NewLine();

			// Check inputs for bad inputs
			if (mileBuf >= 500000) { mileBuf = 0; showMilesageTooBigMsg = true; }
			else { showMilesageTooBigMsg = false; }

			// Show if bad inputs were found
			if (showMilesageTooBigMsg) { ImGui::Text("Mileage out of range. Mileage must be real. \nVehicle was not created"); }

			if (ImGui::BeginPopupModal("Create New Vehicle?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

				if (!CheckStringSize(nameBuf, Vehicle::maxVehicleNameSize)) {
					ImGui::Text("Vehicle name out of range. Max size allowed is %d characters.\nVehicle was not created.", Vehicle::maxVehicleNameSize);
					ImGui::NewLine();
					if (ImGui::Button("Close", ImVec2(120, 30))) { ImGui::CloseCurrentPopup(); }
				}
				else {
					ImGui::NewLine();
					ImGui::Text("This will create a new Vehicle:");
					ImGui::Text("Name:    %s\nMileage: %d", &nameBuf, mileBuf);
					ImGui::NewLine();

					if (ImGui::Button("Create", ImVec2(120, 30))) {
						Vehicle* vehBuf = new Vehicle{ nameBuf, mileBuf };

						addToVehicleList(*vehBuf);
						if (!SaveVehicle(vehBuf)) { ImGui::CloseCurrentPopup(); 	ImGui::OpenPopup("Saving Failed"); }
						else {
							m_showCreateVehicleWin = false;			// Close the Child Window
							showMilesageTooBigMsg = false;		// Reset buffer for future calls to window

							ImGui::CloseCurrentPopup();
						}
						delete vehBuf;
					}
					ImGui::SameLine(); if (ImGui::Button("Close Wihtout Creating", ImVec2(215, 30))) { ImGui::CloseCurrentPopup(); }

				}

				ImGui::EndPopup();
			}

			ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2) - 75);
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 40);
			if (ImGui::Button("Close", ImVec2{ 150, 30 })) { m_showCreateVehicleWin = false; }

			ImGui::EndChild();
		}
		else { m_resetInputBufCreateVehWin = true; }
		// End Create Vehicle Window


	}
	ImGui::End(); //End Vehicle Manager window

	return;
}
bool VehicleManager::SaveVehicle(Vehicle* veh) {
	std::ostringstream saveFileName{ m_directory.string() + veh->getName() + ".dat" };	// Sets the name to the Vehicle's m_name
	std::ostringstream fileText; 															// Buffer of what will be written to the file

	// Vehicle file "Header" portion of name and mileage
	fileText << '(' << veh->getName() << ')';
	fileText << '{' << veh->getMileage() << "}\n";

	for (Repair& repair : veh->getRepairList()) {
		fileText << '<';
		fileText << repair;
		fileText << '>';
		fileText << '\n';
	}
	for (GasStop& gasStop : veh->getGasStopList()) {
		fileText << '[';
		fileText << gasStop;
		fileText << ']';
		fileText << '\n';
	}

	return utl::fs::overwriteFile(saveFileName.str(), fileText.str());
}
bool VehicleManager::DeleteVehicle(Vehicle& veh) {
	delFromVehList(veh); // Delete from s_VehicleList

	// Delete the actual file itself
	std::string deletePath{ m_directory.string() + veh.getName() + ".dat" };
	return utl::fs::deleteFile(deletePath);
}
