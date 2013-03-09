#include <stdlib.h>
#include <stdio.h>
#include "kvapi.h"

do_tests(kvs_handle_t kvh)
{
}

main(int argc, char **argv)
{
	char *uri;
	kvs_handle_t kvh;
	kvs_error_t err;

	if (argc != 2) {
		fprintf(stderr, "Usage: kvtest URI\n");
		exit(1);
	}
	uri = argv[1];
	err = kvstore_open(uri, &kvh);
	if (err || kvh == NULL) {
		fprintf(stderr, "kvstore_open failed err %d\n", err);
		exit(1);
	}
	do_tests(kvh);
	kvs_close(kvh);
	exit(0);
}
