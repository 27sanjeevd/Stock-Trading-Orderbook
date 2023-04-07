#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT 80

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

	// accept
	int sock_client = accept(sock_desc, (struct sockaddr*) &client_info, &client_len);
	if (sock_client < 0) {
		perror("accept");
		return -1;
	}

	char buffer[1024];
	ssize_t recvd = recv(sock_client, buffer, 1024-1, 0);
	if (recvd < 0) {
		perror("receive");
		return -1;
	}
	printf("%s\n", buffer);

	// do client stuff
	const char* output = "Hello World!\n";
	ssize_t sent = send(sock_client, output, strlen(output), 0);
	// clean up

	close(sock_client);
	close(sock_desc);

	return 0;
}