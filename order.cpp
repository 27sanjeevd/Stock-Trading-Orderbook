#include <iostream>
#include <queue>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 80

struct Order {
    float quantity;
    double price;
    bool side;
    int id;
};

struct BuyOrderComparator {
    bool operator()(Order& a, Order& b) {
        return a.price < b.price;
    }
};

struct SellOrderComparator {
    bool operator()(Order& a, Order& b) {
        return a.price > b.price;
    }
};



class Orderbook {
    public:
        int createServer() { 
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
                std::string s = std::string(buff, 0, sizeInBytesOfReceivedData);

                std::istringstream iss(s);
                std::vector<std::string> words;
                std::string word;
                while (std::getline(iss, word, ' ')) {
                    words.push_back(word);
                }

                if (words[0] == "add" && words.size() >= 4) {
                    float quantity = std::stof(words[1]);
                    double price = std::stod(words[2]);
                    bool side;
                    if (words[3] == "true"){
                        side = true;
                    }
                    else {
                        side = false;
                    }
                    addOrder({quantity, price, side});


                    std::string string5 = "order successfully added";
                    const char* messageData1 = string5.c_str();
                    size_t messageSize = string5.length();
                    send(socketClient, messageData1, messageSize, 0);

                }
                else if (words[0] == "print"){
                    printOrders(socketClient);

                    std::string string5 = "order successfully printed";
                    const char* messageData1 = string5.c_str();
                    size_t messageSize = string5.length();
                    send(socketClient, messageData1, messageSize, 0);
                }
                else if (words[0] == "remove" && words.size() >= 2){
                    int id = std::stoi(words[1]);
                    removeOrder(id);
                    

                    std::string string5 = "order successfully removed";
                    const char* messageData1 = string5.c_str();
                    size_t messageSize = string5.length();
                    send(socketClient, messageData1, messageSize, 0);

                }
                else if (words[0] == "match"){
                    matchOrders(socketClient);
                }

                /*
                std::cout << words[0] << std::endl;
                for (const auto& w : words) {
                    std::cout << w << std::endl;
                }
                std::cout << s << std::endl;
                
                std::cout << s.c_str() << std::endl;
                const char* temp = s.c_str();
                std::string string5 = "hello guys, your order";
                const char* messageData1 = string5.c_str();
                size_t messageSize = sizeof(string5);
                send(socketClient, messageData1, messageSize, 0);
                send(socketClient, buff, sizeInBytesOfReceivedData, 0);
                */
                close(socketClient);

            }

            return 0;
        }

        void addOrder(Order order, int id = -1) {
            if (id != -1){
                order.id = id;
            }
            else {
                if (order.side == true){
                    order.id = buy_order += 2;
                }
                else {
                    order.id = sell_order += 2;
                }
            }

            if (order.side == true){
                buys.push(order);
            }
            else {
                sells.push(order);
            }
        }

        void changeQuantity(int id, int quantity){
            if (id % 2 == 0){
                std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> newBuys;

                while (!buys.empty()){
                    Order order = buys.top();
                    buys.pop();
                    if (order.id == id){
                        order.quantity = quantity;
                    }
                    newBuys.push(order);
                }

                buys = newBuys;
            }
            else {
                std::priority_queue<Order, std::vector<Order>, SellOrderComparator> newSells;


                while (!sells.empty()){
                    Order order = sells.top();
                    sells.pop();
                    if (order.id == id){
                        order.quantity = quantity;
                    }
                    newSells.push(order);
                }

                sells = newSells;
            }
        }
        

        void removeOrder(int id){
            if (id % 2 == 0){
                std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> newBuys;

                while (!buys.empty()){
                    Order order = buys.top();
                    buys.pop();
                    if (order.id != id){
                        newBuys.push(order);
                    }
                }

                buys = newBuys;
            }
            else {
                std::priority_queue<Order, std::vector<Order>, SellOrderComparator> newSells;


                while (!sells.empty()){
                    Order order = sells.top();
                    sells.pop();
                    if (order.id != id){
                        newSells.push(order);
                    }
                }

                sells = newSells;
            }
        }

        void printOrders(int socketClient){
            std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> copyBuys = buys;
            std::priority_queue<Order, std::vector<Order>, SellOrderComparator> copySells = sells;

            while (!copyBuys.empty()){
                Order order = copyBuys.top();
                copyBuys.pop();
                std::string s = "Buy ->\tAmount: " + std::to_string(order.quantity) + "\tPrice: " + std::to_string(order.price) + "\tID: " + std::to_string(order.id) + "\n";
                const char* messageData1 = s.c_str();
                size_t messageSize = s.length();
                send(socketClient, messageData1, messageSize, 0);
                std::cout << messageData1;
                //std::cout << "Buy ->\tAmount: " << order.quantity << "\tPrice: " << order.price << "\tID: " << order.id << std::endl;
            }

            while (!copySells.empty()){
                Order order = copySells.top();
                copySells.pop();
                std::string s = "Sell ->\tAmount: " + std::to_string(order.quantity) + "\tPrice: " + std::to_string(order.price) + "\tID: " + std::to_string(order.id) + "\n";
                const char* messageData1 = s.c_str();
                size_t messageSize = s.length();
                send(socketClient, messageData1, messageSize, 0);
                std::cout << messageData1;
                //std::cout << "Sell ->\tAmount: " << order.quantity << "\tPrice: " << order.price << "\tID: " << order.id << std::endl;
            }
        }

        void matchOrders(int socketClient) {
            bool continue1 = true;

            if (buys.size() == 0 || sells.size() == 0){
                continue1 = false;
            }
            
            while (continue1) {
                Order buy = buys.top();
                Order sell = sells.top();
                if (buy.price < sell.price){
                    continue1 = false;
                }
                else {
                    int buy_amt = buy.quantity;
                    int sell_amt = sell.quantity;
                    
                    std::string s = std::to_string(buy.id) + " and " + std::to_string(sell.id) + " matched at amount ";

                    if (buy_amt > sell_amt){
                        buy.quantity -= sell_amt;
                        buys.pop();
                        sells.pop();
                        addOrder(buy, buy.id);
                        s += std::to_string(sell_amt) + "\n";
                        //std::cout << buy.id << " and " << sell.id << " matched at amount " << sell_amt << std::endl;
                    }
                    else if (buy_amt < sell_amt) {
                        sell.quantity -= buy_amt;
                        buys.pop();
                        sells.pop();
                        addOrder(sell, sell.id);
                        s += std::to_string(buy_amt) + "\n";
                        //std::cout << buy.id << " and " << sell.id << " matched at amount " << buy_amt << std::endl;
                    }
                    else {
                        buys.pop();
                        sells.pop();
                        //std::cout << buy.id << " and " << sell.id << " matched at amount " << buy_amt << std::endl;
                    }
                    //printOrders(socketClient);

                    if (buys.size() == 0 || sells.size() == 0){
                        continue1 = false;
                    }
                    
                    const char* messageData1 = s.c_str();
                    size_t messageSize = s.length();
                    send(socketClient, messageData1, messageSize, 0);
                    std::cout << messageData1;
                    
                }
            }
        }

    private:
        std::priority_queue<Order, std::vector<Order>, BuyOrderComparator> buys;
        std::priority_queue<Order, std::vector<Order>, SellOrderComparator> sells;

        int buy_order = 0;
        int sell_order = 1;
};

int main() {
    Orderbook book1;
    int output = book1.createServer();

    return output;
}