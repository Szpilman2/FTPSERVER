#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/value.h> 
//#include "CommandParser.cpp"
#include <jsoncpp/json/json.h>
using namespace std;


void error(string str){
    cout << str << endl;
}


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

class CommandParser{
    public:
        CommandParser(){
            this->commandList.clear();
        }
        vector<string> commandFactory(string command){
            split(command);
                
            if(commandList[0] == "PWD"){
                if (commandList.size() != 1){
                    error("PWD Command Used with invalid number of Arguments.");
                }
                cout << "this is PWD command" << endl;
                return commandList;
            }
            else if (commandList[0] == "MKD"){
                if (commandList.size() != 2){
                    error("MKD command with invalid arguments.");
                    cout << commandList.size() << endl;
                }
                else{
                    cout << commandList.size() << endl;
                }
                return commandList;
            }
            else if (commandList[0] == "DELE"){
                if(commandList[1] == "-d"){
                    cout << "this is a directory delete command." << endl;
                    return commandList;
                }else if(commandList[1] == "-f"){
                    cout << "this is a file delete command." << endl;
                    return commandList;
                }
            }
            else if(commandList[0] == "LS"){
                cout << "this is LS command" << endl;
                return commandList;
            }
            else if(commandList[0] == "CWD"){
                cout << "this is CWD command" << endl;
                return commandList;
            }
            else if (commandList[0] == "RENAME"){
                cout << "this is a Rename command." << endl;
                return commandList;
            }
            else if (commandList[0] == "RETR"){
                cout << "this is a download file command" << endl;
                return commandList;
            }
            else if (commandList[0] == "HELP"){
                cout << "=======================HELP==========================" << endl;
                return commandList;
            }
            else if (commandList[0] == "quit"){
                cout << "this is quit command" << endl;
                return commandList;
            }
            
            cout << "this is an invalid command" << endl;
            return commandList;
            
    }

    void clearBuffer(){this->commandList.clear();}


    private:
        vector<string> commandList;
        void split(string s, string delimiter = " ") {
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            string token;
            while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
                token = s.substr (pos_start, pos_end - pos_start);
                pos_start = pos_end + delim_len;
                this->commandList.push_back (token);
            }

            this->commandList.push_back (s.substr (pos_start));
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
            this -> loggedIn = false;
        }
        void decreaseDownloadCapacity(int amount){
            this -> downloadCapacity -= amount;
        }
        void printUserInfo(){
            cout << this->name << "<==>" << this->password << "<==>" << this->downloadCapacity << endl;
        }
        void setLoggedIn(){
            this->loggedIn = true;
        }
        string getUserName(){
            return this->name;
        }
        string getPassword(){
            return this->password;
        }
        bool getLoginStatus(){
            return this->loggedIn;
        }

    private:
        string name;
        string password;
        int downloadCapacity;
        bool loggedIn;
};

class FTPServer{
    
    public:
        FTPServer() : jsparser("config.json") {
            this->portNumber = this->jsparser.getValueFromJson("Port");
            this->IP = this->jsparser.getValueFromJson("IP");
            this -> addSubscribedUsers();
            this -> getUserInformation();    
        }

        void getUserInformation(){
            string name;
            string pass;
            cout << "Please Enter your username: [provide your information in this format: User <userName>]" << endl ;
            getline(cin, name);
            name = this->refactorInputString(name);
            if(this -> isAuthenticatedUser(name)){
                cout << "331 Password required for:" << name << "[provide your information in this format: Pass <userPassword>]" << endl;
                getline(cin, pass);
                pass = this->refactorInputString(pass);
                if (!(this -> isAuthenticatePassword(name,pass)) || this -> isLogedIn(name)){
                    cout << "503 Bad sequence of commands." << endl;
                    cout << "530 Login incorrect." << endl;
                }else{
                    cout << "230 user " << name << " logged in." << endl;
                    this -> setLogIn(name,pass);
                    this -> getCommand();
                }
            }
            else{
                cout << "530 Login incorrect." << endl;
            }
            
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

        string refactorInputString(string str){
            size_t subStrIndex = 0;
            subStrIndex = str.find(" ", 0);
            str = str.substr(subStrIndex + 1, str.size() - subStrIndex);
            return str;
        }

        bool isAuthenticatedUser(string username){
            for (auto *v : this -> serverUsers){
                if (v->getUserName() == username){
                    return true;
                }
            }
            return false;
        }

        bool isAuthenticatePassword(string username, string password){
            for (auto *v : this -> serverUsers){
                if (v->getUserName() == username){
                    if (v->getPassword() == password){
                        return true;
                    }
                }
            }
            return false;
        }

        bool isLogedIn(string username){
            for (auto *v : this -> serverUsers){
                if (v->getUserName() == username){
                    if (v->getLoginStatus() == true){
                        return true;
                    }
                }
            }
            return false;
        }

        void setLogIn(string username,string password){
            for (auto *v : this -> serverUsers){
                if (v->getUserName() == username){
                    if (v->getPassword() == password){
                        v->setLoggedIn();
                    }
                }
            }
        }
        
};


int main() {
    FTPServer server;
    return 0;
}