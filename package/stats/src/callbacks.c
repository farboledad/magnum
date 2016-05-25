/*
* callbacks.c
*
* Created: August 27th, 2014
*       Author: Franco Arboleda
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "callbacks.h"

static char *statsgenerate(char *json_data, int logged_in, void *context)
{
	char * retval;
        retval = (char *)calloc(BUFFER,sizeof(char));
	json_error_t errors;
	printf("JSON stats generate callback called\n");
        json_t * obj = json_loads(json_data,0,&errors);
	json_t * ret = json_array();
	ret=stats_generate(obj);
	strcat(retval,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}


static char *get_interfaces(char *query_string, int logged_in, void *context)
{
        char * retval;
	retval = (char *)calloc(BUFFER,sizeof(char));
        json_t  * obj=json_object();
	printf("GET active interfaces callback called\n");
        obj=get_active_interfaces();
	strcat(retval,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}

static char *get_hosts(char *query_string, int logged_in, void *context)
{
	char * retval;
        retval = (char *)calloc(BUFFER,sizeof(char));
        json_t  * obj=json_object();
        printf("GET hosts callback called\n");
        obj=get_top_hosts(HOSTFILE);
        strcat(retval,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}

static char *get_hosts_filenames(char *query_string, int logged_in, void *context)
{
        char * retval;
        retval = (char *)calloc(BUFFER,sizeof(char));
        json_t  * obj=json_object();
        printf("GET hosts filenames callback called\n");
        obj=get_hosts_files();
        strcat(retval,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}

static char *get_num_host_per_file(char *json_data, int logged_in, void *context)
{
        char * retval;
        retval = (char *)calloc(BUFFER,sizeof(char));
        json_error_t errors;
        printf("JSON get number of hosts per file callback called\n");
        json_t * obj = json_loads(json_data,0,&errors);
        json_t * ret = json_object();
        ret=num_host_per_file(obj);
        strcat(retval,json_dumps(ret,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}

static char *bck_host_file(char *json_data, int logged_in, void *context)
{
	char * retval;
        retval = (char *)calloc(BUFFER,sizeof(char));
        json_error_t errors;
        printf("JSON bck host file callback called\n");
        json_t * obj = json_loads(json_data,0,&errors);
        json_t * ret = json_object();
        ret=bck_host_file_read(obj);
        strcat(retval,json_dumps(ret,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
        return retval;
}

static char *reset_int_stats(char *query_string, int logged_in, void *context)
{
	char * ret;
	ret = (char *)calloc(BUFFER,sizeof(char));
	json_t * obj=json_object();
	printf("GET reset int stats callback called\n");
	obj = resetintstats();
	strcat(ret,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
	return ret;
}

static char *reset_host_stats(char *query_string, int logged_in, void *context)
{
	char * ret;
        ret = (char *)calloc(BUFFER,sizeof(char));
        json_t * obj=json_object();
	printf("GET reset hosts stats callback called\n");
	obj = resethostsstats();
	strcat(ret,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
	return ret;
}

int register_all()
{
	cg_status_t cg_status;

	cg_status = cg_ui_register_json_callback("simple_json_post_generate_backend", &statsgenerate, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering POST generate callback\n");
                return EXIT_FAILURE;//goto faulty;
        }


        cg_status = cg_ui_register_get_callback("simple_json_get_interfaces_backend", &get_interfaces, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET interfaces callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_get_callback("simple_json_get_hosts_backend", &get_hosts, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET hosts callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_get_callback("simple_json_get_hostsfiles_backend", &get_hosts_filenames, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET hosts filenames callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_json_callback("simple_json_post_numOfHosts_backend",&get_num_host_per_file, "The JSON Context");
	if(cg_status != CG_STATUS_OK){
                printf("Error registering POST get num hosts per file callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_json_callback("simple_json_post_bck_file_backend", &bck_host_file, "The JSON Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering POST bck file callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
	
	cg_status = cg_ui_register_get_callback("simple_json_reset_int_stats_backend", &reset_int_stats, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET reset int stats callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_get_callback("simple_json_reset_hosts_stats_backend", &reset_host_stats, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET reset hosts stats callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
      
        //The url must be relative AND the path must exists.
        //Paths are in /www/your_relative_url
        cg_status = cg_ui_register_page("Statistics", "stats/stats.html");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering stats page\n");
                return EXIT_FAILURE;//goto faulty;
        }
	return EXIT_SUCCESS;
}

int deregister_all()
{
	cg_status_t cg_status;

	cg_status = cg_ui_deregister_page("Statistics");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregistering stats page\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_json_callback("simple_json_post_generate_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister POST generate callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

        cg_status = cg_ui_deregister_get_callback("simple_json_get_interfaces_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET interfaces callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_get_callback("simple_json_get_hosts_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET hosts callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
	
	cg_status = cg_ui_deregister_get_callback("simple_json_get_hostsfiles_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET hosts filenames callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_json_callback("simple_json_post_numOfHosts_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister POST get num hosts per file callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_json_callback("simple_json_post_bck_file_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister POST bck file callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
	
	cg_status = cg_ui_deregister_get_callback("simple_json_reset_int_stats_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET reset int stats callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_get_callback("simple_json_reset_hosts_stats_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET reset hosts stats callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
	
	return EXIT_SUCCESS;
}

