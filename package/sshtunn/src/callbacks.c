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

#include "files_utils.h"
#include "callbacks.h"

static char *json_tunnel_callback(char *json_data, int logged_in, void *context)
{
	json_error_t errors;
	int ret;
        json_t * obj = json_loads(json_data,0,&errors);
        ret=write_file(obj);
	printf("JSON tunnel callback called");
	return (char *)ret;
}

static char *get_tunnel_callback(char *query_string, int logged_in, void *context)
{
        char * retval;
	retval = (char *)calloc(BUFFER,sizeof(char));
        json_t  * obj=json_object();
        obj=read_file(TUNNEL_FILE);
	strcat(retval,json_dumps(obj,JSON_ENSURE_ASCII | JSON_COMPACT | JSON_PRESERVE_ORDER));
	printf("GET TUNNEL callback called");
        return retval;
}

static char *get_cert_callback(char *query_string, int logged_in, void *context)
{
        char * retval;
        retval=create_cert();
        printf("GET CERT callback called");
        return retval;
}

int register_all()
{
	cg_status_t cg_status;
	
	cg_status = cg_ui_register_json_callback("simple_json_post_tunnel_backend", &json_tunnel_callback, "The JSON Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering JSON TUNNEL callback\n");
                return EXIT_FAILURE;
        }

        cg_status = cg_ui_register_get_callback("simple_json_get_tunnel_backend", &get_tunnel_callback, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET TUNNEL callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_register_get_callback("simple_json_get_cert_backend", &get_cert_callback, "The GET Context");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering GET CERT callback\n");
                return EXIT_FAILURE;//goto faulty;
        }
        
        //The url must be relative AND the path must exists.
        //Paths are in /www/your_relative_url
        cg_status = cg_ui_register_page("SSHTunn", "sshtunn/sshtunn.html");
        if(cg_status != CG_STATUS_OK){
                printf("Error registering sshtunn page\n");
                return EXIT_FAILURE;//goto faulty;
        }
	return EXIT_SUCCESS;
}

int deregister_all()
{
	cg_status_t cg_status;

	cg_status = cg_ui_deregister_page("SSHTunn");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregistering sshtunn page\n");
                return EXIT_FAILURE;//goto faulty;
        }

        cg_status = cg_ui_deregister_json_callback("simple_json_post_tunnel_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister JSON TUNNEL callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

        cg_status = cg_ui_deregister_get_callback("simple_json_get_tunnel_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET TUNNEL callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	cg_status = cg_ui_deregister_get_callback("simple_json_get_cert_backend");
        if(cg_status != CG_STATUS_OK){
                printf("Error deregister GET CERT callback\n");
                return EXIT_FAILURE;//goto faulty;
        }

	return EXIT_SUCCESS;
}

