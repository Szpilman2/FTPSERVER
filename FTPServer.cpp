#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include "CommandParser.cpp"
using namespace std;

class FileSystem{
    public:
        FileSystem(string dirPath){
            this->changeWorkingDirectory(dirPath);
            this -> setWorkingDirectory(dirPath);
            this -> setRootDirectory(dirPath);
        }
        
        void RenameFile(const char* oldName, const char* newName){
            rename( oldName, newName);
        }
        void PrintWorkingDirectory(){
            // cout << "Current path is " << filesystem::current_path() << endl; 
            cout << "Current path is: " << this->workingDirectory << endl;
        }
        void listFiles(){
            cout << "contents in this directory are ---> " << endl;
            for (const auto & entry : filesystem::directory_iterator(this->workingDirectory))
                //std::cout << entry.path() << entry.path().filename() <<std::endl;
                std::cout << entry.path().filename() <<std::endl;
        }

        void changeWorkingDirectory(const filesystem::path newPath){
            if (newPath.is_absolute()){
                if (this -> is_path_directory(newPath)){
                    filesystem::current_path(newPath);
                    this->setWorkingDirectory(newPath);
                }
                else{
                    error("Provided Address is not a directory address, please consider some directories...");
                }
                
            }
            else if(newPath.is_relative()){
                filesystem::path fullPath = this->workingDirectory / newPath;
                if(this-> is_path_directory(fullPath)){
                    filesystem::current_path(fullPath);
                    this -> setWorkingDirectory(fullPath);
                }
                else{
                    error("Provided Address is not a directory address, please consider some directories...");
                }
                
            }
        }
        void removeDirectory(string directoryName){
            if(this -> existsDirecotryInPath(directoryName)){
                cout << "deleting this directory ..." << endl;
            }
            else{
                error("Provided directory name is not in current path...");
            }
        }
        void removeFile(string fileName){
            if (this -> existsFileInPath(fileName)){
                cout << "deleting your file..." << endl;
            }
            else{
                error("Provided file name is not in current path...");
            }
        }
        
    private:
        filesystem::path workingDirectory;
        string rootDirectory;
        bool is_path_directory(const filesystem::path newPath){
            filesystem::directory_entry folder(newPath);
            if(folder.is_directory()){
                return true;
            }
            return false;
        }
        void setRootDirectory(filesystem::path newPath){
            this -> workingDirectory = newPath;
        }
        void setWorkingDirectory(filesystem::path newPath){
            this->workingDirectory = newPath;
        }
        bool existsDirecotryInPath(string directoryName){
            for (const auto &entry : filesystem::directory_iterator(this->workingDirectory)){
                if (entry.path().filename() == directoryName){
                    return true;
                }
            }
            return false;
        }
        bool existsFileInPath(string fileName){
            for (const auto &entry : filesystem::directory_iterator(this->workingDirectory)){
                if (entry.path().filename() == fileName){
                    return true;
                }
            }
            return false;
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
    FileSystem filesys("/home/peyman/Desktop/CPP/FTP_Server/FTPSERVER/serverDrive");
    //filesys.RenameFile("test1.txt", "test2.txt");
    //filesys.WorkingDirectory();
    //filesys.listFiles();
    filesys.changeWorkingDirectory("dir2");
    filesys.PrintWorkingDirectory();
    // filesys.listFiles();
    //filesys.removeDirectory("dir2");
    filesys.removeFile("shab.txt");
    return 0;
}