#include "Application.hpp"

int main(int argc, char** argv){
    Dvimana::Application* App = new Dvimana::Application();
    App->Run();
    delete App;
    return 0;
}