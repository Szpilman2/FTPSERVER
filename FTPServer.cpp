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

class JsonParser{
    public:
        JsonParser(string fileName){
            ifstream file(fileName);
            this->reader.parse(file, this->actualJson);
        }
        string getValueFromJson(string str){
            return this->actualJson[str].asString();
        }
        Json::Value getFormattedJson(){
            return this->actualJson;
        }
    private:
        Json::Value actualJson;
        Json::Reader reader;
};

class User{
    public:
        User(string name, string password){
            this -> name = name;
            this -> password = password;
            this -> downloadCapacity = 16000;
        }
        void decreaseDownloadCapacity(int amount){
            this -> downloadCapacity -= amount;
        }
        void printUserInfo(){
            cout << this->name << "<==>" << this->password << "<==>" << this->downloadCapacity << endl;
        }

    private:
        string name;
        string password;
        int downloadCapacity;
};

class FTPServer{
    
    public:
        FTPServer() : jsparser("config.json") {
            this->portNumber = this->jsparser.getValueFromJson("Port");
            this->IP = this->jsparser.getValueFromJson("IP");
            this -> addSubscribedUsers();    
        }
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
        JsonParser jsparser;
        vector<User*> serverUsers;
        string portNumber;
        string IP;

        void addSubscribedUsers(){
            auto formattedJson = jsparser.getFormattedJson();
            for(int index = 1; index <= stoi(this->jsparser.getValueFromJson("UserCount")); index++){
                string userStr = "user" + to_string(index); 
                this->serverUsers.push_back(new User(formattedJson["Users"][userStr]["name"].asString(),
                                                    formattedJson["Users"][userStr]["pass"].asString()));
                userStr = "";
            }
        }
        
};


int main() {
    FTPServer server;
    
    return 0;
}