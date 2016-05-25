/*
* files_utils.h
*
* Created on: Aug 11th, 2014
*	Author: Franco Arboleda
*/

#ifndef FILES_UTILS_H_
#define FILES_UTILS_H_

#include <jansson.h>

#define TUNNEL_FILE "/etc/sshtunn/sshtunn.config"
#define SIZE 254

json_t * read_file(char * fileName);
int write_file(json_t * obj);
char * create_cert();
int file_exists(char *filename);

#endif /* FILES_UTILS_H_ */
