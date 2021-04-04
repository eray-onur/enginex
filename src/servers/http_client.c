#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int http_client(int argc, const char *argv)
{
	char *address;
	address = argv[1];

	int client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Connect to an address.
	struct sockaddr_in remote_address;
	remote_address.sin_family = AF_INET;
	remote_address.sin_port = htons(3131);
	inet_aton(address, &remote_address.sin_addr.s_addr);

	connect(
		client_socket,
		(struct sockaddr *) &remote_address,
		sizeof(remote_address)
	);

	char request[] = "GET / HTTP1.1\r\n\r\n";
	char response[4096];

	// Sending a request and receiving a response.
	send(client_socket, request, sizeof(request), 0);
	recv(client_socket, &response, sizeof(response), 0);

	printf("Response from the server: %s", response);

	pclose(client_socket);

	return 0;
}

