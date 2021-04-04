#include <stdio.h>
#include <stdlib.h>

#include "servers/http_server.h"

int main()
{
	int conn = http_server();
	if(conn == -1)
	{
		printf("Server initialization has failed!");
	}
	return 0;
}
