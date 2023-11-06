#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "CommandParser.cpp"
using namespace std;

class FileSystem{
    public:
        FileSystem(string dirPath){
            workingDirectory = dirPath;
            this->changeWorkingDirectory(dirPath);
        }
        void RenameFile(const char* oldName, const char* newName){
            rename( oldName, newName);
        }
        void PrintWorkingDirectory(){
            // cout << "Current path is " << filesystem::current_path() << endl; 
            cout << "Current path is: " << this->workingDirectory << endl;
        }
        void listFiles(){
            //std::string path = "/path/to/directory";
            for (const auto & entry : filesystem::directory_iterator(this->workingDirectory))
                std::cout << entry.path() << entry.path().filename() <<std::endl;
        }
        void changeWorkingDirectory(const filesystem::path newPath){
            filesystem::current_path(newPath);
        }
    private:
        string workingDirectory;
        //string rootDirectory;
    
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
    FileSystem filesys("/home/peyman/Desktop/CPP/FTP_Server/FTPSERVER/serverDrive");
    //filesys.RenameFile("test1.txt", "test2.txt");
    //filesys.WorkingDirectory();
    //filesys.listFiles();
    filesys.changeWorkingDirectory("dir2");
    filesys.PrintWorkingDirectory();
    return 0;
}