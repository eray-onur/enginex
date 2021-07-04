/*
 * new_http_server.c
 *
 *  Created on: Jun 19, 2021
 *      Author: eray
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../managers/app_manager.h"

struct RequestInfo {
	int* fd_client;
	int* fd_server;
	int* fd_img;
	char buffer[2048];
	struct sockaddr_in* client_addr;
	socklen_t sin_len;
};

char header[] =
"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html; charset=UTF-8;\r\n\r\n";

char json_header[] =
"HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json;charset=UTF-8;\r\n\r\n";

char txt_header[] =
"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/plain;charset=UTF-8;\r\n\r\n";

char* find_route_from_request(char* request_buffer);
void* handle_request(void *req_info);

void * create_server()
{
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buffer[2048];
	int fd_img;
	int fd_script;
	int on = 1;

	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0)
		{
			perror("Server failed to initialize.");
			exit(1);
		}
		setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

		// Initializing server address information such as port number and family.
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;

		uint16_t port = 8080;
		printf("App hosted on port: %d.\n", port);
		server_addr.sin_port = htons(port);

		// Binding server information to specified socket.
		if(bind(fd_server, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
		{
			perror("Bind failed.");
			close(fd_server);
			exit(1);
		}

		// Listening to socket for any incoming requests.
		if(listen(fd_server, 10) == -1)
		{
			perror("Listen error.");
			close(fd_server);
			exit(1);
		}

		struct RequestInfo req_info;
		//req_info.client_addr = client_addr;
		req_info.fd_client = fd_client;
		req_info.fd_server = fd_server;
		req_info.fd_img = fd_img;
		req_info.client_addr = &client_addr;

		struct RequestInfo* req_ptr = &req_info;


		printf("THREAD INITIALIZATION.");

		pthread_t thmaster;
		pthread_t sth1;
		pthread_t sth2;
		pthread_t sth3;

		pthread_create(&thmaster, NULL, handle_request, &req_info);
		pthread_create(&sth1, NULL, handle_request, &req_info);
		pthread_create(&sth2, NULL, handle_request, &req_info);
		pthread_create(&sth3, NULL, handle_request, &req_info);

		pthread_join(sth3, NULL);
		pthread_join(sth2, NULL);
		pthread_join(sth1, NULL);
		pthread_join(thmaster, NULL);

		free(req_ptr);
		return NULL;
}

// HTTP Client Event Loop
void* handle_request(void *req_info)
{
	struct sockaddr_in* server_addr, client_addr;
	struct RequestInfo* info = (struct RequestInfo*)req_info;

	int fd_server = info->fd_server;
	int fd_client = -1;
	int fd_img = info->fd_img;
	socklen_t sin_len = sizeof(info->client_addr);

	char buffer[2048];

	while(1)
	{
		fd_client = accept(fd_server, (struct sockaddr*) &client_addr, &sin_len);
		if(fd_client == -1)
		{
			perror("Connection failed...");
			continue;
		}

		if(!fork())
		{
			// Child process is killed.
			close(fd_server);
			memset(buffer, 0, 2048);
			read(fd_client, buffer, 2047);

			// Extract route information from the request.
			char* route_buffer = find_route_from_request(buffer);
			char dp_root[60] = "./app";
			strcat(dp_root, route_buffer);
			/* Favicon Check */

			if(strstr(dp_root, "./app/index.html"))
			{
				printf("HTML request.\n");
				// Send the requested page to client.
				char text[50000];
				fetch_file_content("./app/index.html", text, 50000);
				printf("Response content: %s\n", text);
				if(text == NULL)
				{
					perror("file not found");
					exit(EXIT_FAILURE);
				}

				char *concat = calloc((strlen(header) + 1 + strlen(text) + 1), sizeof(char));
				if (concat == NULL)
				{
					perror("Calloc");
					exit(EXIT_FAILURE);
				}
				strncat(concat, header, strlen(header));
				strncat(concat, text, strlen(text));

				write(fd_client, concat, strlen(concat) - 1);
				free(concat);
			}
			else if(strstr(dp_root, "./app/dashboard.html"))
			{
				printf("HTML request.\n");
				// Send the requested page to client.
				char text[50000];
				fetch_file_content("./app/dashboard.html", text, 50000);
				printf("Response content: \n%s\n", text);
				if(text == NULL)
				{
					perror("file not found");
					exit(EXIT_FAILURE);
				}

				char *concat = calloc((strlen(header) + 1 + strlen(text) + 1), sizeof(char));
				if (concat == NULL)
				{
					perror("Calloc");
					exit(EXIT_FAILURE);
				}
				strncat(concat, header, strlen(header));
				strncat(concat, text, strlen(text));

				write(fd_client, concat, strlen(concat) - 1);
				free(concat);
			}
			else if(strstr(dp_root, "./app/styles.css"))
			{
				fd_img = open("./app/styles.css", O_RDONLY);
				sendfile(fd_client, fd_img, NULL, 8192);
				close(fd_img);
			}
			else if(strstr(dp_root, "./app/icon.png"))
			{
				fd_img = open("./app/icon.png", O_RDONLY);
				sendfile(fd_client, fd_img, NULL, 8192);
				close(fd_img);
			}
			else if(strstr(dp_root, "./app/data.txt"))
			{
				printf("Plain text request.\n");
				char text[50000];
				fetch_file_content("./app/data.txt", text, 50000);
				printf("Response content: %s\n", text);
				if(text == NULL)
				{
					perror("Text file not found.");
					exit(EXIT_FAILURE);
				}

				char *response = calloc((strlen(txt_header) + 1 + strlen(text) + 1), sizeof(char));
				if (response == NULL)
				{
					perror("Calloc");
					exit(EXIT_FAILURE);
				}
				strncat(response, txt_header, strlen(txt_header));
				strncat(response, text, strlen(text));

				write(fd_client, response, strlen(response) - 1);
			}
			else if(strstr(dp_root, "./app/data.json"))
			{
				printf("JSON text Request.\n");
				char text[50000];
				fetch_file_content("./app/data.json", text, 50000);

				FILE *config = fopen("./app/data.json", "r");
				char configBuffer[512];
				fread(configBuffer, 512, 1, config);
				fclose(config);

				printf("Response content: %s\n", configBuffer);
				if(text == NULL)
				{
					perror("JSON file not found.");
					exit(EXIT_FAILURE);
				}

				char *response = calloc((strlen(json_header) + 1 + strlen(configBuffer) + 1), sizeof(char));
				if (response == NULL)
				{
					perror("Calloc");
					exit(EXIT_FAILURE);
				}
				strncat(response, json_header, strlen(json_header));
				strncat(response, configBuffer, strlen(configBuffer));

				printf("RESPONSE: \n%s", configBuffer);

				write(fd_client, configBuffer, strlen(configBuffer));
			}
			else
			{
				// Send the requested page to client.
				char text[50000];
				fetch_file_content("./error.html", text, 50000);
				printf("Response content: %s\n", text);

				if(text == NULL)
				{
					perror("file not found");
					exit(EXIT_FAILURE);
				}

				char *response = calloc((strlen(header) + 1 + strlen(text) + 1), sizeof(char));
				if (response == NULL)
				{
					perror("Calloc");
					exit(EXIT_FAILURE);
				}
				strncat(response, header, strlen(header));
				strncat(response, text, strlen(text));

				write(fd_client, response, strlen(response) - 1);
				free(response);

			}
			// Deallocate strings in order to prevent memory leaks.
			//free(dp_root);
			//free(buffer);
			free(route_buffer);
			exit(0);
		}
		close(fd_client);
	}
	return NULL;
}

// Parsing the request header for route information.
char* find_route_from_request(char* request_buffer)
{
	size_t current_route_index = 0;
	char *found_route = malloc(100 * sizeof(char));

	size_t slash_index = -1;
	size_t route_endindex = -1;

	size_t request_index = 0;
	size_t length = strlen(request_buffer);

	for (; request_index < length; request_index++) {

	    if(slash_index != -1 && route_endindex != -1)
	    {
	    	break;
	    }

	    if(request_buffer[request_index] == '/')
	    {
	    	slash_index = request_index;
	    }

    	if(slash_index != -1)
    	{
	    	found_route[current_route_index] = request_buffer[request_index];
	    	current_route_index++;
	    	if(request_buffer[request_index] == ' ')
	    	{
	    		route_endindex = request_index;
	    	}
    	}
	}
	return found_route;
}









