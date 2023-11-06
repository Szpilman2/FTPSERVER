#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/value.h> 
#include "CommandParser.cpp"
#include <jsoncpp/json/json.h>
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
                filesystem::remove_all(directoryName);
            }
            else{
                error("Provided directory name is not in current path...");
            }
        }
        void removeFile(string fileName){
            if (this -> existsFileInPath(fileName)){
                filesystem::remove(fileName);
            }
            else{
                error("Provided file name is not in current path...");
            }
        }
        
    private:
        filesystem::path workingDirectory;
        filesystem::path rootDirectory;
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

class JsonParser{
    
    public:
        JsonParser(string fileName){
            ifstream file(fileName);
            this->reader.parse(file, this->actualJson);
        }
        void getValueFromJson(string str){
            cout << this->actualJson[str] << endl;
        }
    private:
        Json::Value actualJson;
        Json::Reader reader;
};

int main() {
    //FTPServer server;
    //server.getCommand();
    //FileSystem filesys("/home/peyman/Desktop/CPP/FTP_Server/FTPSERVER/serverDrive");
    //filesys.RenameFile("test1.txt", "test2.txt");
    //filesys.WorkingDirectory();
    //filesys.listFiles();
    //filesys.changeWorkingDirectory("dir2");
    //filesys.PrintWorkingDirectory();
    // filesys.listFiles();
    //filesys.removeDirectory("dir2");
    //filesys.removeFile("shab.txt");

    // ifstream file("file.json");
    // Json::Value actualJson;
    // Json::Reader reader;

    // Using the reader, we are parsing the json file
    // reader.parse(file, actualJson);

    // The actual json has the json data
    // cout << "Total json data:\n" << actualJson << endl;

    // accessing individual parameters from the file
    // cout << "Name:" << actualJson["Name"] << endl;
    // cout << "Dob:" << actualJson["Dob"] << endl;
    // cout << "College:" << actualJson["College"] << endl;
    JsonParser jparser("file.json");
    jparser.getValueFromJson("College");
    return 0;
}