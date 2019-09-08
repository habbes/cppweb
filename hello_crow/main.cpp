#include "crow_all.h"
using std::cout;

int main (int argc, char *argv[])
{
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/")([](){
        return "<div><h1>Hello, Habbes!</h1></div>";
    });

    char *port = std::getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL ? std::stoi(port) : 18080);
    cout << "PORT = " << iPort << std::endl;
    app.port(iPort).multithreaded().run();
}