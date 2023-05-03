#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include "server_controls.h"
#include "orderbook.h"

#define PORT 80

int num_acc = 0;
int id = 0;
User** users;
OrderBook* server_book = create_orderbook();

int main(int argc, char const *argv[]) {
    // create our socket info structures

    struct sockaddr_in saddr = {0};

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);

    socklen_t server_len = sizeof(saddr);

    struct sockaddr client_info = {0};
    socklen_t client_len = sizeof(client_info);

    // create our socket
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc < 0) {
        perror("socket");
        return -1;
    }

    int reuse = 1;
    setsockopt(sock_desc, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // bind
    if (bind(sock_desc, (struct sockaddr*) &saddr, server_len) < 0) {
        perror("bind");
        return -1;
    }
    // listen
    if (listen(sock_desc, 10) < 0) {
        perror("listen");
        return -1;
    }

    while (1) {
        // accept
        int sock_client = accept(sock_desc, (struct sockaddr*) &client_info, &client_len);
        if (sock_client < 0) {
            perror("accept");
            continue;
        }
        bool log_user = false;
        char buffer[1024];
        User* temp;
        int logout;
        while (1) {
            ssize_t recvd = recv(sock_client, buffer, 1024-1, 0);
            if (recvd < 0) {
                perror("receive");
                break;
            }
            if (recvd == 0) {
                printf("Client disconnected\n");
                break;
            }

            buffer[recvd] = '\0';

            if (strcmp(buffer, "quit\n") == 0) {
                printf("Client requested disconnect\n");
                break;
            }

            char* output;
            if (!log_user) {
	            temp = find_user(users, num_acc, buffer);
	            if (temp == NULL) {
	                num_acc++;
	                users = (User**) realloc(users, sizeof(User*) * num_acc);
	                users[num_acc-1] = (User*) malloc(sizeof(User));

	                temp = create_user(150);
	                *users[num_acc-1] = *temp;
	                output = (char*) malloc(strlen(temp->id) + 1);
	                strcpy(output, temp->id);
	            }
	            else {
	                output = (char*) malloc(strlen(buffer) + 1);
	                strcpy(output, buffer);
	                printf("%s\n", output);
	            	log_user = true;
	            }
	        }
	        else {
	        	output = (char*) malloc(strlen(buffer) + 1);
	        	strcpy(output, buffer);
	        	printf("%s\n", output);
	        	logout = parse_command(temp, output);
	        	if (logout == 1) {
	        		log_user = false;
	        	}
	        	printf("%d\n", temp->num_stocks);
	        }

            // do client stuff
            ssize_t sent = send(sock_client, output, strlen(output), 0);
            if (sent < 0) {
                perror("send");
                break;
            }
            free(output);
        }

        // clean up
        close(sock_client);
    }

    close(sock_desc);
    return 0;
}
