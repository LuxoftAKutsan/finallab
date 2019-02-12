#include "cpp-httplib/httplib.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

using namespace httplib;

int main(void) {
    Server svr;
    svr.Post(R"(/add_file/(\w+))", [](const Request& req, Response& res) {
        std::cout << "add_file" << std::endl;
        auto filename = req.matches[1];
        std::cout << "Filename : " << filename << std::endl;    
        std::cout << "Filesize : " << req.body.length() << std::endl;
        std::ofstream myfile;
        myfile.open (filename);
        myfile << req.body;
        myfile.close();
        res.set_content("Done!", "text/plain");
    });

    svr.Post(R"(/delete_file/(\w+))", [&](const Request& req, Response& res) {
        std::cout << "delete_file" << std::endl;
        auto filename = req.matches[1];
        std::cout << "Filename : " << filename << std::endl;
        char cmd [200];
        int n, a=5, b=3;
        n=sprintf (cmd, "rm %s",filename.str().c_str());
        system(cmd);
        res.set_content("Done!", "text/plain");
    });

    svr.Get(R"(/chown_file/(\w+))", [&](const Request& req, Response& res) {
        std::cout << "chown_file to root" << std::endl;
        auto filename = req.matches[1];
        std::cout << "Filename : " << filename << std::endl;
        if (0 == chown(filename.str().c_str(), 0 ,0)) {
            res.set_content("Done!", "text/plain");
        } else {
            res.set_content("ERROR!", "text/plain");
        }
    });

    svr.Get(R"(/ls)", [&](const Request& req, Response& res) {
        std::cout << "ls" << std::endl;
        std::stringstream ss;
        for (const auto & entry : fs::directory_iterator(".")) {
            ss << entry.path() << std::endl;
        }
        res.set_content(ss.str(), "text/plain");
    });
    std::cout << "http://localhost" << std::endl;
    svr.listen("localhost", 1234);
}
