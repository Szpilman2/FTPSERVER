#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <jsoncpp/json/value.h> 
#include <jsoncpp/json/json.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "tqdm.hpp"
using namespace std;

/*
TODO: delete directory should work for both absolute and relative path. I fixed relative path.
*/

void error(string str){
    cout << str << endl;
}

class FileHandler {
    public:

        static void setFilePath(const string& path){
            filePath = path;
        }

        static void writeToFile(const std::string& content) {

            std::fstream file(FileHandler::filePath, std::ios::out | std::ios::app);
            if (file.is_open()) {
                auto time = std::time(nullptr);
                file << put_time(gmtime(&time), "%c") << " => ";
                file << content << endl;
            } else {
                error("There is a problem in working with files.");
            }

            file.close();
        }

    private:
        static string filePath;
};

string FileHandler::filePath;

class FileSystem{
    public:
        FileSystem(string dirPath){
            this->changeWorkingDirectory(dirPath);
            this -> setWorkingDirectory(dirPath);
            this -> setRootDirectory(dirPath);
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
        
        void RenameFile(const char* oldName, const char* newName){
            rename( oldName, newName);
        }
        string PrintWorkingDirectory(){
            return this->workingDirectory;
        }
        vector<string> listFiles(){
            vector<string> items;
            for (const auto & entry : filesystem::directory_iterator(this->workingDirectory))
                items.push_back(entry.path().filename());
            return items;
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
        void makeNewDirectory(filesystem::path pathDir){
            if (pathDir.is_absolute()){
                filesystem::create_directory(pathDir);  
            }
            else if(pathDir.is_relative()){
                filesystem::path fullPath = this->workingDirectory / pathDir;
                filesystem::create_directory(fullPath);
            }
        }

        bool isPathAbsolute(filesystem::path pathDir){
            return pathDir.is_absolute(); 
        }

        bool isPathRelative(filesystem::path pathDir){
            return pathDir.is_relative();
        }

        string getWorkingDirectory(){
            return this->workingDirectory.u8string();
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

class NetworkHandler{

    public:
        NetworkHandler() : jsparser("config.json"){
            this -> serverSocket = -1;
            this -> clientSocket = -1;
            this -> serverdataSocket = -1;

            serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            serverdataSocket = socket(AF_INET, SOCK_STREAM, 0);

            if (serverSocket == -1 || serverdataSocket == -1) {
                cout << "Server: Socket creation failed" << std::endl;
                return;
            }
            serverAddress.sin_family = AF_INET;
            //serverAddress.sin_port = htons(stoi(this->jsparser.getValueFromJson("Port")));
            serverAddress.sin_port = htons(12345);
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            //serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
            //serverAddress.sin_addr.s_addr = this->jsparser.getValueFromJson("IP");

            if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
                std::cerr << "Server: Bind failed" << std::endl;
                return;
            }

            if (listen(serverSocket, 5) == -1) {
                std::cerr << "Server: Listen failed" << std::endl;
                return;
            }

            cout << "Server is listening for connections..." << std::endl;
            FileHandler::writeToFile("Server is UP and is listening for connections ...");

            clientSocket = accept(serverSocket, nullptr, nullptr);
            if (clientSocket == -1) {
                std::cerr << "Server: Accept failed" << std::endl;
                return;
            }

            //------------------------------------------------------------------------------
            dataAddress.sin_family = AF_INET;
            dataAddress.sin_addr.s_addr = INADDR_ANY;
            //dataAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 
            dataAddress.sin_port = htons(12543);

            if (bind(serverdataSocket, (struct sockaddr*)&dataAddress, sizeof(dataAddress)) == -1) {
                std::cerr << "Server: Bind failed" << std::endl;
                return;
            }

            if (listen(serverdataSocket, 5) == -1) {
                std::cerr << "Server: Listen failed" << std::endl;
                return;
            }

            clientdataSocket = accept(serverdataSocket, nullptr, nullptr);
            if (clientdataSocket == -1) {
                std::cerr << "Server: Accept failed" << std::endl;
                return;
            }

        }

        void sendData(const std::string& data) {
            if (clientSocket != -1) {
                send(clientSocket, data.c_str(), data.length(), 0);
            }
        }

        string receiveData() {
            char buffer[1024];
            ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead <= 0) {
                return "";
            }
            buffer[bytesRead] = '\0';
            return std::string(buffer);
        }

        void sendFile(const std::string& filePath) {
            std::ifstream file(filePath, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cerr << "Server: Unable to open file: " << filePath << std::endl;
                return;
            }

            std::streamsize fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            //Send the file size as a binary value
            send(clientSocket, reinterpret_cast<const char*>(&fileSize), sizeof(fileSize), 0);

            // send the file name to the client.
            std::string fileName = filesystem::path(filePath).filename();
            const char* fileNameCStr = fileName.c_str();
            size_t fileNameSize = fileName.size() + 1;  // Include the null terminator
            send(clientSocket, fileNameCStr, fileNameSize, 0);


            // Prepare a buffer to read and send the file in chunks
            const std::size_t bufferSize = 1024;
            char buffer[bufferSize];

            // Send the file in chunks
            while (!file.eof()) {
                file.read(buffer, bufferSize);
                send(clientdataSocket, buffer, file.gcount(), 0);
            }
            file.close();
        }


    private:
        JsonParser jsparser;
        int serverSocket;
        int serverdataSocket;
        int clientSocket;
        int clientdataSocket;
        sockaddr_in serverAddress;
        sockaddr_in dataAddress;
};

class CommandParser{
    public:
        CommandParser(string serverRootDirectory) {
            serverfilesystem = new FileSystem(serverRootDirectory);
            networkHandler = new NetworkHandler();
            this->commandList.clear();
        }
        NetworkHandler* getNetworkHandler(){
            return this->networkHandler;
        }
        bool commandFactory(string command){
            split(command);
                
            if(commandList[0] == "PWD"){
                if (commandList.size() != 1){
                    networkHandler->sendData("PWD Command Used with invalid number of Arguments.");
                }
                else{
                    networkHandler->sendData("257 " + serverfilesystem->PrintWorkingDirectory() + " is the current directory.");
                    FileHandler::writeToFile("User has entered command: PWD");
                }
                return false;
            }
            else if (commandList[0] == "MKD"){
                if (commandList.size() != 2){
                    networkHandler->sendData("MKD command used with invalid Number of arguments.");
                }
                else{
                    serverfilesystem->makeNewDirectory(commandList[1]);
                    if(serverfilesystem->isPathAbsolute(commandList[1])){
                        networkHandler->sendData("257 " + commandList[1] + " directory created.");
                    }
                    else if(serverfilesystem->isPathRelative(commandList[1])){
                        networkHandler->sendData("257 " + serverfilesystem->getWorkingDirectory() + "/" + commandList[1] + " directory created.");
                    }
                    FileHandler::writeToFile("User has entered command: MKD " + commandList[1]);
                }
                return false;
            }
            else if (commandList[0] == "DELE"){
                if(commandList[1] == "-d"){
                    serverfilesystem->removeDirectory(commandList[2]);
                    networkHandler->sendData("250 " + commandList[2] + " directory deleted.");
                    FileHandler::writeToFile("User has entered command: DELE -D " + commandList[2]);
                    return false;
                }
                else if(commandList[1] == "-f"){
                    serverfilesystem->removeFile(commandList[2]);
                    networkHandler->sendData("250 " + commandList[2] + " file deleted.");
                    FileHandler::writeToFile("User has entered command: DELE -f " + commandList[2]);
                    return false;
                }
                else{
                    networkHandler->sendData("DELE argeuments are -d or -f, you entered wrong...");
                    return false;
                }
            }
            else if(commandList[0] == "LS"){
                string items;
                for (auto v : serverfilesystem->listFiles()){
                    items = items + v + "\t";
                }
                networkHandler->sendData("contents in this directory are ===> \n" + items);
                FileHandler::writeToFile("User has entered command: LS");
                return false;
            }
            else if(commandList[0] == "CWD"){
                if (commandList.size() != 2){
                    networkHandler->sendData("CWD command should have 2 arguments.");
                }
                else{
                    serverfilesystem->changeWorkingDirectory(commandList[1]);
                    networkHandler->sendData("250 Directory changed to " + commandList[1]);
                    FileHandler::writeToFile("User has entered command: CWD " + commandList[1]);
                }
                return false;
            }
            else if (commandList[0] == "RENAME"){
                if (commandList.size() != 3){
                    networkHandler->sendData("RENAME command should have 3 arguments.");
                }
                else{
                    serverfilesystem->RenameFile(commandList[1].c_str(), commandList[2].c_str());
                    networkHandler->sendData("250 Rename successful: " + commandList[1] + " renamed to " + commandList[2]);
                    FileHandler::writeToFile("User has entered command: RENAME " + commandList[1] + " " + commandList[2]);
                }
                return false;
            }
            else if (commandList[0] == "RETR"){
                if (commandList.size() != 2){
                    networkHandler->sendData("RETR command should have 2 arguments.");
                }
                else{
                    if (serverfilesystem->existsFileInPath(commandList[1])){
                        networkHandler->sendFile(serverfilesystem->PrintWorkingDirectory()+"/"+commandList[1]);
                        networkHandler->sendData("sent file successfully ...");
                    }
                }
                FileHandler::writeToFile("User has entered command: RETR");
                return false;
            }
            else if (commandList[0] == "HELP"){
                const string longString = R""""(
                ==========================================HELP==================================================
                you can enter following commands:
                    PWD  : prints working directory.
                    LS   : list all files in the current path.
                    MKD directory_name : creates directory_name in current path.
                    DELE [-d] [-f] dir/file_name : deletes directory or file in current path.
                    RENAME oldfile newfile : renames oldfile to newfile.
                    RETR file_name : downloads file_name from FTP server if the file exists in the current path. 
                    quit : for closing the program.
                )"""";
                networkHandler->sendData(longString);
                FileHandler::writeToFile("User has entered command: HELP");
                return false;
            }
            else if (commandList[0] == "quit"){
                networkHandler->sendData("221 Goodbye!");
                FileHandler::writeToFile("User has entered command: quit");
                return true;
            }
            
            cout << "this is an invalid command" << endl;
            return false;
            
    }

    void clearBuffer(){this->commandList.clear();}


    private:
        vector<string> commandList;
        FileSystem* serverfilesystem;
        NetworkHandler* networkHandler;
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
            
            this->parser = new CommandParser(this->jsparser.getValueFromJson("rootDirectory"));
            this->portNumber = this->jsparser.getValueFromJson("Port");
            this->IP = this->jsparser.getValueFromJson("IP");
            this -> addSubscribedUsers();
            this -> getUserInformation();    
        }

        ~ FTPServer() {
            FileHandler::writeToFile("User " + this->getLoggedInUser() + " logged out.");
        }

        void getUserInformation(){
            string name;
            string pass;
            this->parser->getNetworkHandler()->sendData("Please Enter your username: [provide your information in this format: User <userName>]");
            name = this->parser->getNetworkHandler()->receiveData();
            name = this->refactorInputString(name);
            if(this -> isAuthenticatedUser(name)){
                
                this->parser->getNetworkHandler()->sendData("331 Password required for: " + name + " [provide your information in this format: Pass <userPassword>]");
                pass = this->parser->getNetworkHandler()->receiveData();
                pass = this->refactorInputString(pass);
                if (!(this -> isAuthenticatePassword(name,pass)) || this -> isLogedIn(name)){
                    this->parser->getNetworkHandler()->sendData("503 Bad sequence of commands.");
                    this->parser->getNetworkHandler()->sendData("530 Login incorrect.");
                }
                else{
                    this->parser->getNetworkHandler()->sendData("230 user " + name + " logged in.\n" + "**** Type Your Commands ****");
                    FileHandler::writeToFile("User " + name + " logged in.");
                    this -> setLogIn(name,pass);
                    this -> getCommand();
                }
            }
            else{
                this->parser->getNetworkHandler()->sendData("530 Login incorrect.");
            }
            
        }
        void getCommand(){
            string str;
            bool isQuit = false;

            while(!isQuit){
                str = this->parser->getNetworkHandler()->receiveData();
                bool isQuit = this->parser->commandFactory(str);
                if (isQuit) {break;}
                this->parser->clearBuffer();
            }
        }

    private:
        CommandParser* parser;
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

        string getLoggedInUser(){
            for (auto *v: this -> serverUsers){
                if (this->isLogedIn(v->getUserName())){
                    return v->getUserName();
                }
            }
            return "";
        }
        
};


int main() {
    FileHandler::setFilePath("/home/peyman/Desktop/CPP/FTP_Server/FTPSERVER/log.txt");
    FTPServer server;
    // std::vector<int> A = {1,2,3,4,5,6};
    
    // for (int a : tq::tqdm(A))
    // {
    //     // do some heavy work, (e.g. sleep(1) to test, and #include <unistd.h>)
    //     sleep(1);
    // }
    // cout << endl;
    return 0;
}