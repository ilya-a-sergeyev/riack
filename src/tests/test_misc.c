
#include "test.h"

#define TEST_NON_EXISTING_KEY "fairly_big_chance_of_not_existing_123321"

int test_misc(char* testcase)
{
	if (strcmp(testcase, "serverinfo") == 0) {
		return test_server_info();
	} else if (strcmp(testcase, "error") == 0) {
		return test_last_error();
	} else if (strcmp(testcase, "options") == 0) {
		return test_connect_with_options();
	} else {
		return -1;
	}
}

int test_connect_with_options()
{
	struct RIACK_CLIENT *client;
	struct RIACK_CONNECTION_OPTIONS options;
	int result;
	result = 1;
	client = riack_new_client(0);
	options.recv_timeout_ms = 1500;
	options.send_timeout_ms = 1500;
	if (riack_connect(client, test_host, test_port, &options) == RIACK_SUCCESS) {
		result = 0;
	}
	riack_free(client);
	return result;
}

int test_last_error()
{
	RIACK_STRING bucket, key;
	struct RIACK_MAPRED_RESULT *result;
	bucket.len = strlen(RIAK_TEST_BUCKET);
	bucket.value = RIAK_TEST_BUCKET;
	key.len = strlen(TEST_NON_EXISTING_KEY);
	key.value = TEST_NON_EXISTING_KEY;
	// This is just plain wrong! (which is the general idea when testing error handling)
	if (riack_map_redue(test_client, strlen(RIAK_TEST_BUCKET), RIAK_TEST_BUCKET, APPLICATION_ERLANG_TERM, &result) == RIACK_ERROR_RESPONSE) {
		if (strlen(test_client->last_error) > 0)
			return 0;
	}
	return 1;
}

int test_server_info()
{
	RIACK_STRING node, version;
	if (riack_server_info(test_client, &node, &version) == RIACK_SUCCESS) {
		riack_free_string(test_client, &node);
		riack_free_string(test_client, &version);
		return 0;
	}
	return 1;
}
