#include <iostream>
#include <sstream>
#include <vector>
#include "CommandParser.cpp"
using namespace std;

class FTPServer{
    
    public:
        void getCommand(){
            string str;
            while(getline(cin, str)){
                vector<string> result = this->parser.commandFactory(str);
                if (result[0] == "quit") {break;}
                this->parser.clearBuffer();
            }
        }

    private:
        CommandParser parser;
};

int main() {
    FTPServer server;
    server.getCommand();
    return 0;
}