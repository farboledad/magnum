/*
* callbacks.h
*
* Created on: Aug 27th, 2014
*       Author: Franco Arboleda
*/

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>
#include <libcg/cg_conf.h>

#include "files_utils.h"

#define BUFFER 2048

static char *json_tunnel_callback(char *json_data, int logged_in, void *context);
static char *get_tunnel_callback(char *query_string, int logged_in, void *context);
static char *get_cert_callback(char *json_data, int logged_in, void *context);
int register_all();
int deregister_all();

#endif /* CALLBACKS_H_ */
