#ifndef _GLOBAL_CONTEXT_H
#define _GLOBAL_CONTEXT_H

#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>

#include "model/userList.h"
#include "network/networkInfo.h"
#include "logger/logger.h"
#include "ai/aiEngine.h"
#include "model/userRequestQueue.h"

struct GlobalContext {
  SemaphoreHandle_t littlefsMutex;
  Adafruit_NeoPixel* onboardRGBP;
  Adafruit_NeoPixel* externalRGBP;
  UserList users;
  NetworkInfo networkInfo;
  Logger logger;
  AsyncWebServer* webserverP;
  AIEngine aiEngine;
  UserRequestQueue userReqQ;
  LiquidCrystal_I2C* lcdP;
};

#endif