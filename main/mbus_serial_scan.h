#include "string.h"
#include "stdio.h"
#include "mbus.h"

static int debug = 1;

int ping_address(mbus_handle *handle, mbus_frame *reply, int address);

int mbus_scan();