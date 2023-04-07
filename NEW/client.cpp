#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 80

int main(int argc, char const *argv[]) {
	//create server server_info

	struct sockaddr_in server_info = {0};
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	server_info.sin_port = htons(PORT);

	socklen_t server_len = sizeof(server_info);

	//create socket

	int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_desc < 0) {
		perror("socket");
		return -1;
	}

	//connect to server with socket
	if (connect(sock_desc, (struct sockaddr*) &server_info, server_len) < 0) {
		perror("connect");
		return -1;
	}

	const char* output = "Hello World!\n";
	ssize_t sent = send(sock_desc, output, strlen(output), 0);


	char buffer[1024];
	ssize_t recvd = recv(sock_desc, buffer, 1024-1, 0);
	if (recvd < 0) {
		perror("receive");
		return -1;
	}
	printf("%s\n", buffer);


	close(sock_desc);
	return 0;
}