/* EXAMPLE:
*  cg_ui
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <libcg/cg_general.h>
#include <libcg/cg_ui.h>
#include <libcg/cg_conf.h>

#include "files_utils.h"
#include "callbacks.h"

int libcg_initialized = 0;
int forked = 0;

void
handleSig(int sig) {
	int ret;
	printf("Signal received %d\n", sig);
	ret = deregister_all();
	printf("Return from deregister_all is %d\n",ret);
	cg_deinit();
	system("killall sshtunn");
	printf("Process Killed!!!\n");
}

int daemonize()
{
	
	pid_t pid;

#ifdef CG_LIB
	if (libcg_initialized) cg_deinit();
	libcg_initialized = 0;
#endif

	/* Fork off the parent process */
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		printf("Unable to fork: %s\n", strerror(errno));
		return -1;
	}

	/* Success: Let the parent terminate */
	if (pid > 0)
		exit(EXIT_SUCCESS);

	/* On success: The child process becomes session leader */
	if (setsid() < 0) {
		printf("setsid(): %s\n", strerror(errno));
		return -1;
	}
	/* Catch, ignore and handle signals */
	//TODO: Implement a working signal handler */
	signal(SIGCHLD, handleSig);
	signal(SIGHUP, handleSig);
	/* Fork off for the second time*/
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		printf("Unable to fork: %s\n", strerror(errno));
		return -1;
	}

	/* Success: Let the parent terminate */
	if (pid > 0)
		exit(EXIT_SUCCESS);

	/* Set new file permissions */
	umask(0);

	/* Change the working directory to the root directory */
	/* or another appropriated directory */
	chdir("PACKAGE_RUNNING_DIRECTORY");

	/* Close all open file descriptors */
	/*
	int x;
	for (x = sysconf(_SC_OPEN_MAX); x > 0; x--)
			{
		close(x);
	}
	*/
	/*
	x = open("/dev/null", O_RDWR);
	dup(0);
	dup(0);

	lockfd = open(PACKAGE_LOCK_FILE, O_RDWR | O_CREAT, 0640);

	if (lockfd < 0) {
		printf("Unable to open lock file: %s\n", strerror(errno));
		return -1;
	}

	if (lockf(lockfd, F_TLOCK, 0) < 0) {
		LOGD("Another process exists");
		return -1;
	}

	char str[10];
	snprintf(str, 10, "%d\n", getpid());

	write(lockfd, str, strlen(str));
	*/

	forked = 1;
#ifdef CG_LIB
	cg_init("sshtunn");
	libcg_initialized = 1;
#endif
	return 0;
}

int main(void)
{
	int ret;
	
	cg_init("sshtunn");
	libcg_initialized = 1;
	
	if(daemonize)
	{
		int32_t level;
		cg_get_api_level(&level);
		printf("SDK API level: %d\n", level);
		ret = register_all();
		printf("Return of register_all() is = %d\n",ret);
		if(ret == 0)
		{
			while(1)
			{
				sleep(2);
			}
		}
	}
	return 0;
}
