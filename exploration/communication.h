#ifndef COMM_H 
#define COMM_H
#include <app_channel.h>
#endif

struct PacketRX {
	uint8_t order;
} __attribute__((packed));

struct testPacketTX {
	uint8_t test;
} __attribute__((packed));
