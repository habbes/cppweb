#include "crow_all.h"
using std::cout;

int main (int argc, char *argv[])
{
    crow::SimpleApp app;
    
    CROW_ROUTE(app, "/")([](const crow::request &req, crow::response &res){
        std::ifstream in("../public/index.html", std::ifstream::in);
        if (in) {
            std::ostringstream contents;
            contents << in.rdbuf();
            in.close();
            res.write(contents.str());
        }
        else {
            res.write("Not found.");
        }
        res.end();
    });

    char *port = std::getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL ? std::stoi(port) : 18080);
    cout << "PORT = " << iPort << std::endl;
    app.port(iPort).multithreaded().run();
}