#include "gtest/gtest.h"
#include <trutle/Trutle.h>


// Start the server using the default configuration.
// Returns 0 on success, < 0 on failure. Only one server started by 
// this functioncan be running at any given time.
int start_server();

// Attempt to kill the server started by start_server(), if
// one such exists.
void kill_server();

