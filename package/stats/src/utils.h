/*
* utils.h
*
* Created on: Aug 11th, 2014
*	Author: Franco Arboleda
*/

#ifndef UTILS_H_
#define UTILS_H_

#include <jansson.h>

#define SIZE 254
#define HOSTFILE "/tmp/statistics/hosts.txt"
#define HOSTSFILES "/tmp/statistics/darkstat/files.txt"
#define BACKUP_LOCATION "/rom/mnt/cust/etc/backupdata"

json_t * get_active_interfaces();
json_t * get_top_hosts();
json_t * get_hosts_files();
json_t * num_host_per_file(json_t * obj);
json_t * bck_host_file_read(json_t * obj);
json_t * stats_generate(json_t * obj);
json_t * resetintstats();
json_t * resethostsstats();

void data_backup();

#endif /* UTILS_H_ */
