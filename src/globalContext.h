#ifndef _GLOBAL_CONTEXT_H
#define _GLOBAL_CONTEXT_H

#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>

#include "model/userList.h"
#include "network/networkInfo.h"
#include "logger/logger.h"

struct GlobalContext {
  SemaphoreHandle_t littlefsMutex;
  Adafruit_NeoPixel onboardRGB;
  UserList users;
  NetworkInfo networkInfo;
  Logger logger;
  AsyncWebServer* webserverP;
};

#endif