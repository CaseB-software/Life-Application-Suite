#ifndef DEBUG_LOG_H
#define DEBUG_LOG_H

#include "Module.h"

#include <sstream>
#include <iomanip>
#include <chrono>


// Interact with and view logs created by the application
// AS OF 6 NOV 2022, DOES NOT WORK


class Debug : public Module {
public:
    Debug();
    ~Debug() override;

    void    run() override;
    bool    init()   override;

   std::vector<std::string>* m_logs;
};


#endif