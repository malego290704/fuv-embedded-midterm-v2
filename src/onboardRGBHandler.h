#ifndef _ONBOARD_RGB_HANDLER_H
#define _ONBOARD_RGB_HANDLER_H

#include <Adafruit_NeoPixel.h>

#include "globalContext.h"

void taskOnboardRGBHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskOnboardRGBHandler");
  Logger* loggerP = &contextP->logger;
  Adafruit_NeoPixel light = *contextP->onboardRGBP;
  loggerP->log(LOGGER_INFO, "Init Onboard RGB");
  light.begin();
  for (;;) {
    light.fill(0xffffff);
    light.show();
    vTaskDelay(pdMS_TO_TICKS(1000));
    light.fill(0);
    light.show();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}

#endif