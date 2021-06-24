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
#include <pthread.h>

#include "servers/http_server.h"

int main(int argc, char *argv[])
{
	// Get the global variables from config.
	FILE *config = fopen("./eginx.config.json", "r");
	char configBuffer[512];
	fread(configBuffer, 512, 1, config);
	fclose(config);

	// Get the app directory.
	FILE* file = fetch_file_from_route("example_app");
	// serve the html file according to request.

	create_server();
	return 0;
}
