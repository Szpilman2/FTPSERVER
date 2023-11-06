#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "CommandParser.cpp"
using namespace std;

class FileSystem{
    public:
        void RenameFile(const char* oldName, const char* newName){
            rename( oldName, newName);
        }

    
};



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
    //FTPServer server;
    //server.getCommand();
    FileSystem filesys;
    filesys.RenameFile("test1.txt", "test2.txt");
    return 0;
}