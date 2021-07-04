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
	// P.S: Config file is not used in any way at its current state.
	FILE *config = fopen("./eginx.config.json", "r");
	char configBuffer[512];
	fread(configBuffer, 512, 1, config);
	fclose(config);

	// Get the app directory.
	//FILE* file = fetch_file_from_route("example_app");
	// bind to a hardcoded port for any requests.

	// Initialize and bind a unit of work to four threads.
	create_server();
//	pthread_t thmaster;
//	pthread_t sth1;
//	pthread_t sth2;
//	pthread_t sth3;
//
//	pthread_create(&thmaster, NULL, create_server, NULL);
//	pthread_create(&sth1, NULL, create_server, NULL);
//	pthread_create(&sth2, NULL, create_server, NULL);
//	pthread_create(&sth3, NULL, create_server, NULL);
//
//	// Wait for the executing threads to quit the application.
//	pthread_join(sth1, NULL);
//	pthread_join(sth2, NULL);
//	pthread_join(sth3, NULL);
//	pthread_join(thmaster, NULL);

	exit(0);
}
