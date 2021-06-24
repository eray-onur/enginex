/*
 * app_manager.h
 *
 *  Created on: Jun 19, 2021
 *      Author: eray
 */

#ifndef APP_MANAGER_H_
#define APP_MANAGER_H_

void fetch_file_content(char* path, char* buffer, size_t buffer_size);

void app_list(const char* dirname);

char* find_app(const char * app_name);

char* fetch_file(const char * filename);

FILE* fetch_file_from_route(const char * apppath);

#endif /* APP_MANAGER_H_ */
