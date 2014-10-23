#ifndef LOGICALLINK_H
#define LOGICALLINK_H

#define F 0x7E
#define A 0x03
#define C 0x03

#include <signal.h>

//connection establishment variables
int retry_count = 0;
int connecting = 1;

void llopen();

#endif
