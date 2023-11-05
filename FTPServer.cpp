#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

void error(string str){
    cout << str << endl;
}

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