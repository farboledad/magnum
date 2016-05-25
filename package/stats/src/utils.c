/*
* utils.c
*
* Created: August 11th, 2014
*	Author: Franco Arboleda
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <libcg/cg_general.h>
#include <libcg/cg_net.h>

#include "utils.h"

#define BUFFER 20
#define LINE_SIZE 2048
#define LINE_BUFFER 100
#define SLEEP 1

json_t * get_active_interfaces()
{
	int num, i, count_int=0;
	cg_net_if_t *interfaces = NULL;
	json_t * ret=json_object();
	char * key;
	char * value;
	key = (char *)calloc(BUFFER,sizeof(char));
	value = (char *)calloc(BUFFER,sizeof(char));

        cg_net_get_interface_list (&num, &interfaces);
	
        for (i=0; i<num; i++) {
	        cg_net_if_t *iface = &interfaces[i];
		if (iface->status & CG_NET_INTERFACE_ENABLED)
		{	
			printf("Interface %s enabled!\n",iface->dev_name);
			sprintf(key,"%d",count_int);
			sprintf(value,"%s",iface->dev_name);
			json_object_set_new(ret,key,json_string(value));
			count_int++;
		}
        }
	json_object_set_new(ret,"num",json_integer(count_int));
        free(interfaces);
	free(key);
	free(value);
	return(ret);
}

json_t * get_top_hosts(char *filename)
{
	FILE * pFile;
	char * line;
	char * running;
	char * piece;
        char * ip;
        char * hostname;
        char * mac;
        char * dl;
        char * ul;
        char * total;
        char * ls;
        int i;
	json_t * ret=json_array();
        line = (char *)calloc(LINE_SIZE,sizeof(char));
	running = (char *)calloc(SIZE,sizeof(char));
        pFile=fopen(filename,"r");
	while(!feof(pFile))
        {
                fscanf(pFile,"%s",line);
                if(*line)
		{
			json_t * obj = json_object();
		if(strstr(line, "Running") != NULL){
			for(i=0;i<strlen(line);i++)
			{
				if(line[i] == ';') sprintf(running,"%s ",running);
				else sprintf(running,"%s%c",running,line[i]);
			}
			json_object_set_new(obj,"running",json_string(running));
		}
		else
		{	
                        i=0;
                        piece = strtok(line,";");
                        while (piece != NULL)
                        {
                                switch(i)
                                {
                                        case 0:ip=piece;break;
                                        case 1:hostname=piece;break;
                                        case 2:mac=piece;break;
                                        case 3:dl=piece;break;
                                        case 4:ul=piece;break;
					case 5:total=piece;break;
                                        case 6:ls=piece;break;
                                        default:printf("Unknown item!\n");break;
                                }
                                piece=strtok(NULL,";");
                                i++;
                        }
                	json_object_set_new(obj,"ip",json_string(ip));
                        json_object_set_new(obj,"hostname",json_string(hostname));
                        json_object_set_new(obj,"mac",json_string(mac));
                        json_object_set_new(obj,"dl",json_string(dl));
                        json_object_set_new(obj,"ul",json_string(ul));
                        json_object_set_new(obj,"total",json_string(total));
                        json_object_set_new(obj,"ls",json_string(ls));
		}
			json_array_append_new(ret,obj);
		}
                memset(line,0,LINE_SIZE*sizeof(char));
        }
        fclose(pFile);
	free(line);
	free(running);
        return(ret);
}

json_t * get_hosts_files()
{
	FILE * pFile;
	char * line;
        int i, count_int=0;
	json_t * ret=json_array();
        char * key;
	line = (char *)calloc(LINE_SIZE,sizeof(char));
	key = (char *)calloc(BUFFER,sizeof(char));

	pFile=fopen(HOSTSFILES,"r");
	while(!feof(pFile))
        {
                fscanf(pFile,"%s",line);
                if(*line)
		{
			json_t * obj=json_object();
                       	sprintf(key,"%d",count_int);
                        json_object_set_new(obj,key,json_string(line));
               	        count_int++;
			json_array_append_new(ret,obj);
		}
                memset(line,0,LINE_SIZE*sizeof(char));
        }
        fclose(pFile);
	free(line);
	free(key);
        return(ret);
}

json_t * num_host_per_file(json_t * obj)
{
	char * interface = (char *)json_string_value(json_object_get(obj,"int"));
	char * date;
        date = (char *)calloc(12,sizeof(char));
	char * filename;
        filename = (char *)calloc(SIZE,sizeof(char));
	json_t * array = json_array();
	array = json_object_get(obj,"values");
	size_t size = json_array_size(array);
	printf("The size of the array is: %d\n",size);
	int count = 0;
	json_t * ret = json_array();
	for(count;count<size;count++)
	{
		date = (char *)json_string_value(json_array_get(array,count));
		if(strcmp(date,"Current")==0)
		{
			sprintf(filename,"%s",HOSTFILE);
		}
		else
		{		
			sprintf(filename,"/etc/statistics/darkstat/%s_hosts_%s.txt",date,interface);
		}
		printf("Date is: %s\nFilename is: %s\n",date,filename);
		printf("Amount of hosts in file: %d\n",countHostLines(filename));
		json_t * obj=json_object();
                json_object_set_new(obj,date,json_integer(countHostLines(filename)));
                json_array_append_new(ret,obj);
		memset(date,0,12*sizeof(char));
		memset(filename,0,SIZE*sizeof(char));
	}	
	return(ret);
}

int countHostLines (char * filename)
{
	FILE * pFile;
	char * line;
        line = (char *)calloc(LINE_SIZE,sizeof(char));
	int lines=0;
        pFile=fopen(filename,"r");
	while(!feof(pFile))
        {
		fscanf(pFile,"%s",line);
                if(*line)
                {
			lines++;
                }
                memset(line,0,LINE_SIZE*sizeof(char));
	}
	if(lines > 0)
	{
		lines-=1; //Removing the header line from the count
	}
	else
	{
		lines=0;
	}
	fclose(pFile);
	return(lines);
}

json_t * bck_host_file_read(json_t * obj)
{
	char * interface = (char *)json_string_value(json_object_get(obj,"int"));
	char * date = (char *)json_string_value(json_object_get(obj,"hostfile"));
	char * filename;
	filename = (char *)calloc(SIZE,sizeof(char));
	sprintf(filename,"/etc/statistics/darkstat/%s_hosts_%s.txt",date,interface);
	//printf("This is the file name to read: %s\n",filename);
	json_t * ret = json_array();
	ret = get_top_hosts(filename);
	free(filename);
	return(ret);
}

json_t * stats_generate(json_t * obj)
{
        char * interface = (char *)json_string_value(json_object_get(obj,"interface"));
        char * command;
	srand(time(NULL));
	int r = rand();
	json_t * ret=json_array();
	char * picname;
        command = (char *)calloc(SIZE,sizeof(char));
	picname = (char *)calloc(SIZE,sizeof(char));
        sprintf(command,"/rom/mnt/cust/etc/vnstatigenerate %s %d",interface,r);
        system(command);
        sprintf(command,"/rom/mnt/cust/etc/darkstatgenerate %s",interface);
	system(command);
	//Copying files from darkstat backup
	system("rm -rf /etc/statistics/darkstat/files.txt");
	sprintf(command,"echo Current > /tmp/statistics/darkstat/files.txt && ls /etc/statistics/darkstat/*_%s.txt|sed 's/\\/etc\\/statistics\\/darkstat\\///g'|sed 's/\_hosts\_.\*.txt//g' >> /tmp/statistics/darkstat/files.txt",interface);
	//printf("This is the command: %s\n",command);
	system(command);
	//
        free(command);
	//Compression
	convert_to_csv(interface);
	sprintf(picname,"vnstat_d_%d.png",r);
	json_array_append_new(ret,json_string(picname));
        sprintf(picname,"vnstat_h_%d.png",r);
        json_array_append_new(ret,json_string(picname));
	sprintf(picname,"vnstat_m_%d.png",r);
        json_array_append_new(ret,json_string(picname));
	sprintf(picname,"vnstat_s_%d.png",r);
        json_array_append_new(ret,json_string(picname));
	sprintf(picname,"vnstat_t_%d.png",r);
        json_array_append_new(ret,json_string(picname));
	json_object_set(obj,"images",ret);
	compress_for_download(interface);
	free(picname);
	return(ret);
}

json_t * resetintstats()
{
	json_t * ret=json_object();
	system("/etc/init.d/vnstat stop");
        system("rm -rf /etc/statistics/vnstat/* && rm -rf /tmp/statistics/vnstat/*");
        system("/etc/init.d/vnstat start");
	json_object_set_new(ret,"return",json_string("OK"));
        return(ret);
}

json_t * resethostsstats()
{
	json_t * ret=json_object();
	system("rm -rf /etc/statistics/darkstat/*");
	system("/etc/init.d/darkstat restart");
        json_object_set_new(ret,"return",json_string("OK"));
        return(ret);
}

void data_backup()
{
	char * command = BACKUP_LOCATION;
	system(command);
}

void compress_for_download(char * interface)
{
	char * command;
	FILE * pFile;
        command = (char *)calloc(LINE_SIZE,sizeof(char));
	char * filename="download.tar.gz";
	char * line;
        line = (char *)calloc(LINE_SIZE,sizeof(char));

	sprintf(command,"rm -rf /tmp/statistics/%s",filename);
	system(command);
	sprintf(command,"tar -C /tmp/statistics/ -czf /tmp/statistics/%1$s vnstat_%2$s_dumped.csv hosts_%2$s.csv vnstat/%2$s",filename,interface);

	pFile=fopen(HOSTSFILES,"r");
        while(!feof(pFile))
        {
                fscanf(pFile,"%s",line);
                if(*line)
                {
			if(strcmp(line,"Current")!=0)
			{
				printf("File detected from %s\n",line);
				sprintf(command,"%s darkstat/%s_hosts_%s.csv",command,line,interface);
			}
                }
                memset(line,0,LINE_SIZE*sizeof(char));
        }
        fclose(pFile);
	printf("This is the compression command: %s\n",command);
	system(command);
	free(command);
	free(line);
}

void convert_to_csv(char * interface)
{
	char * readfilename;
	char * writefilename;
	char * command;
	readfilename = (char *)calloc(SIZE,sizeof(char));
	writefilename = (char *)calloc(SIZE,sizeof(char));
	command = (char *)calloc(SIZE,sizeof(char));
	sprintf(readfilename,"/tmp/statistics/vnstat_%s_dumped.txt",interface);
	sprintf(writefilename,"/tmp/statistics/vnstat_%s_dumped.csv",interface);
	//Generating CSV file for vnstat and the selected interface
	sprintf(command,"cp /rom/mnt/cust/etc/vnstat_dumped.csv /tmp/statistics/vnstat_%s_dumped.csv",interface);
	system(command);
	vnstat_to_csv(readfilename,writefilename);
	sprintf(readfilename,"/tmp/statistics/hosts_%s.txt",interface);
        sprintf(writefilename,"/tmp/statistics/hosts_%s.csv",interface);
	sprintf(command,"/rom/mnt/cust/etc/hosts_csvgen %s %s",readfilename,writefilename);
	system(command);
	sprintf(command,"/rom/mnt/cust/etc/hosts_bck_csvgen %s",interface);
	system(command);
	free(readfilename);
	free(writefilename);
	free(command);
}

void vnstat_to_csv(char * readfilename, char * writefilename)
{
	FILE * pFile;
        FILE * wFile;
        char * line;
        char * writeline;
        char * piece;
        char * array;
        char iface[15];
        long dlint;
        long dldec;
        long ulint;
        long uldec;
        double dltotal;
        double ultotal;
        int location, size;
        long dl, ul, dlk, ulk;
        char * dldata;
        char * uldata;
        double days[30][2];
        double months[12][2];
        double top[10][2];
        long hours[24][2];
        int i;
        line = (char *)calloc(LINE_BUFFER,sizeof(char));
        writeline = (char *)calloc(LINE_SIZE,sizeof(char));
        dldata = (char *)calloc(LINE_BUFFER,sizeof(char));
        uldata = (char *)calloc(LINE_BUFFER,sizeof(char));

	pFile=fopen(readfilename,"r");

        while(!feof(pFile))
        {
                fscanf(pFile,"%s",line);
                if(*line)
                {
                        if(strstr(line,"interface")){
                                i=0;

                                piece = strtok(line,";");
                                while (piece != NULL)
                                {
                                        switch(i)
                                        {
                                                case 0:break;
                                                case 1: strncpy(iface, (char *)piece,15);
                                                default:break;
                                        }
                                        piece=strtok(NULL,";");
                                        i++;
                                }
                        }
			else if(strstr(line,"totalrx")){
                                i=0;

                                piece=strtok(line,";");
                                if(strstr(line,"totalrxk")){
                                        while (piece != NULL)
                                        {
                                                switch(i)
                                                {
                                                        case 0:break;
                                                        case 1:dldec=atol(piece);break;
                                                        default:printf("Unknown item!\n");break;
                                                }
                                               piece=strtok(NULL,";");
                                                i++;
                                        }
                                        sprintf(dldata,"%ld.%ld",dlint,dldec);
                                        dltotal=atof(dldata);
                                }
                                else
                                {
                                        while (piece != NULL)
                                        {
                                                switch(i)
                                                {
                                                        case 0:break;
                                                        case 1:dlint=atol(piece);break;
                                                        default:printf("Unknown item!\n");break;
                                                }
                                                piece=strtok(NULL,";");
                                                i++;
                                        }
                                }
                        }
			else if(strstr(line,"totaltx")){
                                i=0;

                                piece = strtok(line,";");
                                if(strstr(line,"totaltxk")){
                                        while (piece != NULL)
                                        {
                                                switch(i)
                                                {
                                                        case 0:break;
                                                        case 1:uldec=atol(piece);break;
                                                        default:printf("Unknown item!\n");break;
                                                }
                                                piece=strtok(NULL,";");
                                                i++;
                                        }
                                        sprintf(uldata,"%ld.%ld",ulint,uldec);
                                        ultotal=atof(uldata);
                                }
                                else{
                                        while (piece != NULL)
                                        {
                                                switch(i)
                                                {
                                                        case 0:break;
                                                        case 1:ulint=atol(piece);break;
                                                        default:printf("Unknown item!\n");break;
                                                }
                                                piece=strtok(NULL,";");
                                                i++;
                                        }
                                }
                        }
			else if (strstr(line,"d;") || strstr(line,"m;") || strstr(line,"t;")){
                                if (!(strstr(line,"created") || strstr(line,"updated")))
                                {
                                        i=0;
                                        dl=0;
                                        ul=0;
                                        dlk=0;
                                        ulk=0;

                                        piece = strtok(line,";");
                                        while (piece != NULL)
                                        {
                                                switch(i)
                                                {
                                                        case 0:array=piece;break;
                                                        case 1:location=atol(piece);break;
                                                        case 2:break;
                                                        case 3:dl=atol(piece);break;
                                                        case 4:ul=atol(piece);break;
                                                        case 5:dlk=atol(piece);break;
                                                        case 6:ulk=atol(piece);break;
                                                        case 7:break;
                                                        default:printf("Unknown item!\n");break;
                                                }
                                                piece=strtok(NULL,";");
                                                i++;
                                        }
                                        sprintf(dldata,"%ld.%ld",dl,dlk);
                                        sprintf(uldata,"%ld.%ld",ul,ulk);
                                        if(strcmp(array,"d")==0){
                                                days[location][0]=atof(dldata);
                                                days[location][1]=atof(uldata);
                                        }
                                        else if(strcmp(array,"m")==0){
                                                months[location][0]=atof(dldata);
                                                months[location][1]=atof(uldata);
                                        }
                                        else if(strcmp(array,"t")==0){
                                                top[location][0]=atof(dldata);
                                                top[location][1]=atof(uldata);
                                        }
                                }
                        }
			else if(strstr(line,"h;"))
                        {
                                i=0;
                                dl=0;
                                ul=0;

                                piece = strtok(line,";");
                                while (piece != NULL)
                                {
                                        switch(i)
                                        {
                                                case 0:array=piece;break;
                                                case 1:location=atol(piece);break;
                                                case 2:break;
                                                case 3:dl=atol(piece);break;
                                                case 4:ul=atol(piece);break;
                                                default:printf("Unknown item!\n");break;
                                         }
                                         piece=strtok(NULL,";");
                                         i++;
                                }
                                if(strcmp(array,"h")==0){
                                        hours[location][0]=dl;
                                        hours[location][1]=ul;
                                }
                        }
                }
                memset(line,0,LINE_BUFFER*sizeof(char));
        }
        fclose(pFile);
        free(line);
	//writting part
	wFile = fopen(writefilename,"a");
        sprintf(writeline,"%s\t%lf\t%lf",iface,dltotal,ultotal);
        size = (int)(sizeof(days)/sizeof(days[0]));
        for(i=0;i<size;i++)
        {
                sprintf(writeline,"%s\t%lf\t%lf",writeline,days[i][0],days[i][1]);
        }
        size = (int)(sizeof(months)/sizeof(months[0]));
        for(i=0;i<size;i++)
        {
                sprintf(writeline,"%s\t%lf\t%lf",writeline,months[i][0],months[i][1]);
        }
        size = (int)(sizeof(top)/sizeof(top[0]));
        for(i=0;i<size;i++)
        {
                sprintf(writeline,"%s\t%lf\t%lf",writeline,top[i][0],top[i][1]);
        }
        size = (int)(sizeof(hours)/sizeof(hours[0]));
        for(i=0;i<size;i++)
        {
                sprintf(writeline,"%s\t%ld\t%ld",writeline,hours[i][0],hours[i][1]);
        }
        fputs(writeline,wFile);
        fclose(wFile);
        free(writeline);
}
