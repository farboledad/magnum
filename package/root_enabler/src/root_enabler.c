#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <libcg/cg_general.h>

int main (int argc, char *argv[])
{
	cg_status_t ret;
	int status = EXIT_FAILURE;
	char *password = NULL;

	if ((ret = cg_init("root_enabler")) != CG_STATUS_OK) {
		cg_system_log(CG_LOG_ERR, "cg_init failed with status %d", ret);
		exit(status);
	}

	if ((ret = cg_get_serial_number(&password)) != CG_STATUS_OK) {
		cg_system_log(CG_LOG_ERR, "cg_get_serial_number failed with status %d", ret);
	}
	if (!password || strlen(password) < 2) {
		cg_system_log(CG_LOG_ERR, "Failed to get serial number, using default password");
		password = strdup("correcthorsebatterystaple");
	}

	if ((ret = cg_set_dev_mode(TRUE, CG_DEV_ROOT_ACCESS, password)) != CG_STATUS_OK) {
		cg_system_log(CG_LOG_ERR, "cg_set_dev_mode failed with status %d", ret);
		goto error;
	}

	status = EXIT_SUCCESS;
error:
	free(password);
	cg_deinit();
	exit(status);
}

