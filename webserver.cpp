#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 80

int main() {

    // SERVER
    struct sockaddr_in saddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = INADDR_ANY,
        .sin_port = htons(PORT)
    };
    int option = 1;
    int saddrSize = sizeof(saddr);
    int socketServer = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM -> TCP. SOCK_DGRAM -> UDP
    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));

    if (socketServer == -1){
        std::cerr << "Not able to create our socket" << std::endl;
        return -1;
    }

    // CLIENT
    struct sockaddr_in caddr;
    socklen_t caddrSize = sizeof(caddr);
    int socketClient;


    //Binding the socket to the ip+port
    bind(socketServer, (struct sockaddr*) &saddr, sizeof(saddr));

    //Listening
    listen(socketServer, SOMAXCONN); //SOMAXCONN enables maximum connections possible, which is 128
    //Print
    std::stringstream ss;
    ss << PORT;
    std::cout << "[Server] Listening on port " << ss.str() << std::endl;
    

    char buff[4096];
    int sizeInBytesOfReceivedData;

    //WHILE waiting for client
    while (true) {
        //Accept connections from clients
        socketClient = accept(socketServer, (struct sockaddr*)&caddr, (socklen_t*)&caddrSize);
        std::cout << "[Server] Client connected successfully" << std::endl;

        //Try to find out who is the client
        char hostClient[NI_MAXHOST];
        char portClient[NI_MAXSERV];
        memset(hostClient, 0, NI_MAXHOST);
        memset(portClient, 0, NI_MAXSERV);
        if (getnameinfo((sockaddr*)&caddr, sizeof(caddr), hostClient, NI_MAXHOST, portClient, NI_MAXSERV, 0) == 0){
            std::cout << "--> " << hostClient << " connected to the port " << portClient << std::endl;
        }
        else {
            inet_ntop(AF_INET, &caddr.sin_addr, hostClient, NI_MAXHOST);
            std::cout << "--> " << hostClient << " connected to the port " << ntohs(caddr.sin_port) << std::endl;
        }

        //Receive data
        sizeInBytesOfReceivedData = recv(socketClient, buff, 4096, 0);
        if (sizeInBytesOfReceivedData == -1){
            std::cerr << "Error receiving message. Quitting";
            break;
        }
        else if (sizeInBytesOfReceivedData == 0){
            std::cout << "Client Disconnected" << std::endl;
            break;
        }
        send(socketClient, buff, sizeInBytesOfReceivedData + 1, 0);

        std::cout << std::string(buff, 0, sizeInBytesOfReceivedData) << std::endl;

        close(socketClient);
    }



    return 0;
}