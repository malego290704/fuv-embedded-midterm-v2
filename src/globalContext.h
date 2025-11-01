#ifndef _GLOBAL_CONTEXT_H
#define _GLOBAL_CONTEXT_H

#include <Adafruit_NeoPixel.h>

#include "model/userList.h"
#include "network/networkInfo.h"

struct GlobalContext {
  SemaphoreHandle_t littlefsMutex;
  Adafruit_NeoPixel onboardRGB;
  UserList users;
  NetworkInfo networkInfo;
};

#endif