/*
* files_utils.c
*
* Created: August 11th, 2014
*	Author: Franco Arboleda
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "files_utils.h"

#define BUFFER 2048
#define SLEEP 1

json_t * read_file(char * fileName)
{
	FILE * pFile;
	char * line;
	char * piece;
	char * user;
	char * serIP;
	char * serPort;
	char * monPort;
	char * start;
	int i;
	json_t * ret=json_object();
	line = (char *)calloc(SIZE,sizeof(char));
	pFile=fopen(fileName,"r");
	while(!feof(pFile))
	{
		fscanf(pFile,"%s",line);
		if(*line)
		{	json_t * obj = json_object();
			i=0;
                        piece = strtok(line,":");
                        while (piece != NULL)
                        {
                                switch(i)
        	                {
                	                case 0:user=piece;break;
                                        case 1:serIP=piece;break;
                                        case 2:serPort=piece;break;
					case 3:monPort=piece;break;
					case 4:start=piece;break;
                                        default:printf("Unknown item!\n");break;
                                }
                                piece=strtok(NULL,":");
                                i++;
                        }
                        json_object_set_new(ret,"user",json_string(user));
                        json_object_set_new(ret,"serIP",json_string(serIP));
                        json_object_set_new(ret,"serPort",json_string(serPort));
                        json_object_set_new(ret,"monPort",json_string(monPort));
			if(strcmp(start,"true")==0) json_object_set_new(ret,"start",json_true());
			else json_object_set_new(ret,"start",json_false());
		}
		memset(line,0,SIZE*sizeof(char));
	}	
	fclose(pFile);
	free(line);
	return(ret);
}

int write_file(json_t * obj)
{
        char * user;
	char * serIP;
	char * serPort;
	char * monPort;
	char * start;
	char * uci_add;
	uci_add = (char *)calloc(BUFFER,sizeof(char));
	//Deleting the old configuration
	system("uci delete sshtunnel.sshtunnel_config.user");
	system("uci delete sshtunnel.sshtunnel_config.serIP");
	system("uci delete sshtunnel.sshtunnel_config.serPort");
	system("uci delete sshtunnel.sshtunnel_config.monPort");
	system("uci delete sshtunnel.sshtunnel_config.start");
	sleep(SLEEP);
        system("uci commit sshtunnel");
        //
        user=(char *)json_string_value(json_object_get(obj,"user"));
	sprintf(uci_add,"uci set sshtunnel.sshtunnel_config.user=\"%s\"",user);
	//printf("The uci line is: %s\n",user);
	system(uci_add);
        serIP=(char *)json_string_value(json_object_get(obj,"serIP"));
        sprintf(uci_add,"uci set sshtunnel.sshtunnel_config.serIP=\"%s\"",serIP);
	//printf("The uci line is: %s\n",serIP);
	system(uci_add);
	serPort=(char *)json_string_value(json_object_get(obj,"serPort"));
	sprintf(uci_add,"uci set sshtunnel.sshtunnel_config.serPort=\"%s\"",serPort);
	//printf("The uci line is: %s\n",serPort);
	system(uci_add);
	monPort=(char *)json_string_value(json_object_get(obj,"monPort"));
        sprintf(uci_add,"uci set sshtunnel.sshtunnel_config.monPort=\"%s\"",monPort);
        //printf("The uci line is: %s\n",monPort);
        system(uci_add);
	if(json_is_true(json_object_get(obj,"start"))) start = "true";
	else start = "false";
        sprintf(uci_add,"uci set sshtunnel.sshtunnel_config.start=\"%s\"",start);
        //printf("The uci line is: %s\n",start);
        system(uci_add);
	//
        system("uci commit sshtunnel");
	sleep(SLEEP);
	system("/etc/init.d/sshtunn restart");
	return 0;
}

char * create_cert()
{
	char * ret;
	ret = (char *)calloc(4096,sizeof(char));
	FILE * pFile;
	if(file_exist("/etc/dropbear/authorized_keys"))
	{
		pFile=fopen("/etc/dropbear/authorized_keys","r");
 		fgets(ret,4096,pFile);
	}
	else
	{
		system("/usr/bin/dropbearkey -y -f /etc/dropbear/dropbear_rsa_host_key|grep ssh-rsa > /etc/dropbear/authorized_keys");
	        pFile=fopen("/etc/dropbear/authorized_keys","r");
                fgets(ret,4096,pFile);
	}
	//printf("This is the cert: %s\n",ret);
	return ret;
}

int file_exist (char *filename)
{
  struct stat buffer;   
  return (stat (filename, &buffer) == 0);
}
