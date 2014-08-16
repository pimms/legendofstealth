#include "Tests.h"
#include "gtest/gtest.h"
#include <unistd.h>

int main(int argc, char **argv) 
{
	char **fakev = new char*[argc + 1];
	for (int i=0; i<argc; i++) {
		fakev[i] = argv[i];
	}

	fakev[argc] = new char[32];
	strcpy(fakev[argc++], "--gtest_color=yes");

	::testing::InitGoogleTest(&argc, fakev);
	return RUN_ALL_TESTS();
}





/* 
================
Server Management 
================
*/
static int server_pid = 0;

int start_server()
{
	if (server_pid != 0) {
		return -1;
	}

	server_pid  = fork();
	if (server_pid < 0) {
		Log::Error("Failed to fork server: %i", server_pid);

		int ret = server_pid;
		server_pid = 0;
		return ret;
	}

	if (!server_pid) {
		execl("./srv", "");
		Log::Error("FAILURE IN STARTING SERVER: KILLING PARENT");
		kill(getppid(), 9);
		exit(-1);
	} 	

	return 0;
}

void kill_server()
{
	if (server_pid) {
		kill(server_pid, 9);
		server_pid = 0;
	}
}
