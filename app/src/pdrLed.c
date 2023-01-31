#include "app.h"
#include "app_channel.h"

#include "debug.h"

#include "ledseq.h"

#define DEBUG_MODULE "HELLOWORLD"

struct testLedPacket {
  float value;
} __attribute__((packed));

// struct testPacketTX {
//   float msg;
// } __attribute__((packed));

static const float one = 1;

void appMain()
{
  DEBUG_PRINT("Waiting for activation ...\n");

  struct testLedPacket ledStruct;
//   struct testPacketTX txPacket;

  while(1) {
    if (appchannelReceivePacket(&ledStruct, sizeof(ledStruct), 0)) {

        if(ledStruct.value == one){
            for(int i = 0; i<100; i++){
                ledseqTest();
            }
        }
    }
  }
}