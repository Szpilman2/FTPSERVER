#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
using namespace std;

/*
change color in c++ terminal:
    const std::string red("\033[0;31m");
    const std::string green("\033[1;32m");
    const std::string yellow("\033[1;33m");
    const std::string cyan("\033[0;36m");
    const std::string magenta("\033[0;35m");
    const std::string reset("\033[0m");
*/

class Client {
public:
    Client(const char* serverIP, int port) : clientSocket(-1) {
        
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Client: Socket creation failed" << std::endl;
            return;
        }

        
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(12345); 
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); 

        
        if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Client: Connection failed" << std::endl;
            return;
        }
    }

    ~Client() {
        close(clientSocket);
    }

    void sendData(const std::string& data) {
        if (clientSocket != -1) {
            send(clientSocket, data.c_str(), data.length(), 0);
        }
    }

    std::string receiveData() {
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            return "";
        }
        buffer[bytesRead] = '\0'; 
        return std::string(buffer);
    }

    int getClientSocket(){
        return this->clientSocket;
    }


    void receiveFileFromServer() {
        
        std::streamsize fileSize;
        recv(clientSocket, reinterpret_cast<char*>(&fileSize), sizeof(fileSize), 0);
        cout << "Client: fileSize: " << fileSize << endl;

        char fileNameBuffer[1024];
        recv(clientSocket, fileNameBuffer, sizeof(fileNameBuffer), 0);
        std::string fileName(fileNameBuffer);
        std::ofstream outFile(fileName, std::ios::binary);
        cout << "Client: fileName: " << fileName << endl;

        // Receive and write the file in chunks
        const std::size_t bufferSize = 1024;
        char buffer[bufferSize];
        std::streamsize bytesRead;
        std::streamsize totalBytesRead = 0;

        while (totalBytesRead < fileSize) {
            bytesRead = recv(clientSocket, buffer, std::min(fileSize - totalBytesRead, static_cast<std::streamsize>(bufferSize)), 0);

            if (bytesRead <= 0) {
                break;  // Exit the loop when there is nothing more to receive or an error occurs
            }

            outFile.write(buffer, bytesRead);
            totalBytesRead += bytesRead;
        }

        outFile.close();
    }



private:
    int clientSocket;
    sockaddr_in serverAddress;
};

int main() {
    Client client("127.0.0.1", 12345);
    const std::string cyan("\033[0;36m");
    const std::string green("\033[1;32m");;

    if (client.getClientSocket() != -1) {
        
        bool valid = true;
        string message;
        
        while(valid){

            std::string receivedData = client.receiveData();
            if (!receivedData.empty()) {
                std::cout << green << "Received from server: " << receivedData << std::endl;
                if (receivedData == "quit"){
                    valid = false;
                    break;
                }
            }
            if (message == "quit"){
                    valid = false;
                    break;
            }
            cout << cyan << "Enter Command: \t"; 
            getline(cin, message);
            client.sendData(message);

            if (message.find("RETR") != std::string::npos){
                client.receiveFileFromServer();
            }
        }
    }

    return 0;
}
