/*
 * 		app_manager.c
 *
 *  	Created on: Jun 19, 2021
 *      Author: eray
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

void fetch_file_content(const char *path, char* buffer, size_t buffer_size)
{

	FILE* file;

	if((file = fopen(path, "r")) == NULL)
	{
		perror("Failed to open file.");
		return;
	}
	else
	{
		fread(buffer, buffer_size, 1, file);
		fclose(file);
	}
}

void app_list(const char* dirname)
{
	struct dirent *entry;
	DIR *dp = opendir(dirname);

	char * app_names[200] = {0};
	size_t app_index = 0;

	if (dp == NULL)
	{
		perror("Failed to read apps directory.");
	    return;
	}

	entry = readdir(dp);

	while(entry != NULL)
	{
		char *blank = {0};

		blank = strcasestr(entry->d_name, ".");

		if(!blank)
		{
			char* app_name = entry->d_name;
			app_names[app_index] = app_name;
			app_index += 1;
		}
		entry = readdir(dp);
	}

	closedir(dp);
}

char* find_app(char * app_name)
{
	char root_d[200] = "./apps/";
	strcat(root_d, app_name);
	printf("App to search: %s\n", root_d);

	struct dirent *entry;
	DIR *dp = opendir(root_d);

	if (dp == NULL)
	{
		perror("Failed to read apps directory.");
		return NULL;
	}

	entry = readdir(dp);

	while(entry != NULL)
	{
		char *found_app = {0};

		found_app = strcasestr(entry->d_name, app_name);


		if(found_app)
		{
			char* app_name = entry->d_name;
			return app_name;
		}
		entry = readdir(dp);
	}

	closedir(dp);
	return NULL;
}

FILE* fetch_file_from_route(const char * apppath)
{
	char root_d[200] = "./apps/";
	strcat(root_d, apppath);
	printf("App to search: %s\n", root_d);

	struct dirent *entry;
	DIR *dp = opendir(root_d);

	if (dp == NULL)
	{
		perror("Failed to read apps directory.");
		return NULL;
	}

	entry = readdir(dp);

	while(entry != NULL)
	{
		char *blank = {0};
		blank = strcasestr(entry->d_name, "..");
		if(!blank)
		{
			char* app_name = entry->d_name;
			return app_name;
		}
		entry = readdir(dp);
	}

	closedir(dp);
	return NULL;
}

