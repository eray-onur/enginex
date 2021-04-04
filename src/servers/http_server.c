#include "../servers/http_server.h"

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

/*
 * Only sends an index.html file found on source folder. Will be improved upon further iterations.
*/
int http_server()
{
	// Open served html.
	FILE *html_file;
	char *html_data;
	size_t file_size;
	html_file = fopen("./index.html", "r");

	if(html_file == NULL)
	{
		printf("Could not find the entry file! Aborting the server...");
		return -1;
	}

	// Proceed to read file.

	fseek(html_file, 0L, SEEK_END);
	file_size = ftell(html_file); // Determine the file size.
	fseek(html_file, 0L, SEEK_SET);

	html_data = malloc(file_size + 1);

	size_t size = fread(html_data, 1, file_size, html_file);
	html_data[size] = 0;



	// Setting up the response header.
	char http_header[2048] = "HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n\r\n";
	// Concatenating stream data into header.
	strcat(http_header, html_data);

	// If the file stream is still available, close it in order to prevent memory leak.
	if(html_file != NULL)
	{
		fclose(html_file);
	}

	// Create a socket.
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	// Define the socket address.
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(3131);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket address.
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	listen(server_socket, 500);

	// Continuously listen to requests until interrupt.
	int client_socket;
	while(1)
	{
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, http_header, sizeof(http_header), 0);
		free(html_data); // Deallocate the file data from memory.
		close(client_socket);
	}




	return 0;
}
