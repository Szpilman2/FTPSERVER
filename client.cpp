#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

class Client {
public:
    Client(const char* serverIP, int port) : clientSocket(-1) {
        // Create a socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cerr << "Client: Socket creation failed" << std::endl;
            return;
        }

        // Server address and port to connect to
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(12345); // Server port number
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address

        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Client: Connection failed" << std::endl;
            return;
        }
    }

    ~Client() {
        // Close the client socket
        close(clientSocket);
    }

    void sendData(const std::string& data) {
        if (clientSocket != -1) {
            send(clientSocket, data.c_str(), data.length(), 0);
        }
    }

    // Receive data from the server
    std::string receiveData() {
        char buffer[1024];
        ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            return "";
        }
        buffer[bytesRead] = '\0'; // Null-terminate the received data
        return std::string(buffer);
    }

    int getClientSocket(){
        return this->clientSocket;
    }

private:
    int clientSocket;
    sockaddr_in serverAddress;
};

int main() {
    Client client("127.0.0.1", 12345);  // Connect to the server at IP address "127.0.0.1" and port 12345

    if (client.getClientSocket() != -1) {
        // Client is connected to the server

        // Send data to the server
        //client.sendData("Hello, Server!");
        bool valid = true;
        string message;
        // Receive data from the server
        while(valid){
            std::string receivedData = client.receiveData();
            if (!receivedData.empty()) {
                std::cout << "Received from server: " << receivedData << std::endl;
                if (receivedData == "quit"){
                    valid = false;
                    break;
                }
            }
            getline(cin, message);
            client.sendData(message);
        }
    }

    return 0;
}
