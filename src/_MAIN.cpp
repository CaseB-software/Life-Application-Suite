#include "Application.h"


int main(int, char**) {

    Application& app{ Application::getInstance() };

    try{
        app.init();
        app.run();
    }
    catch(...){
        return -1;
    }

    return 0;
}