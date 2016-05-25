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

#include "utils.h"

#define BUFFER 2048

static char *statsgenerate(char *json_data, int logged_in, void *context);
static char *get_interfaces(char *query_string, int logged_in, void *context);
static char *get_hosts(char *json_data, int logged_in, void *context);
static char *get_hosts_filenames(char *query_string, int logged_in, void *context);
static char *get_num_host_per_file(char *json_data, int logged_in, void *context);
static char *bck_host_file(char *json_data, int logged_in, void *context);
static char *reset_int_stats(char *query_string, int logged_in, void *context);
static char *reset_host_stats(char *query_string, int logged_in, void *context);

int register_all();
int deregister_all();

#endif /* CALLBACKS_H_ */
