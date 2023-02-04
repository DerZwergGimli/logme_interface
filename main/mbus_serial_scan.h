#include "string.h"
#include "stdio.h"
#include "mbus.h"


int ping_address(mbus_handle *handle, mbus_frame *reply, int address);

int mbus_scan_old();