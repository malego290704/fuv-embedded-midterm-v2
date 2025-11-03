#ifndef _EXTERNAL_RGB_HANDLER_H
#define _EXTERNAL_RGB_HANDLER_H

#include <Adafruit_NeoPixel.h>

#include "globalContext.h"

void taskExternalRGBHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskExternalRGBHandler");
  Logger* loggerP = &contextP->logger;
  Adafruit_NeoPixel light = *contextP->externalRGBP;
  loggerP->log(LOGGER_INFO, "Init External RGB");
  light.begin();
  for (;;) {
    light.fill(0xffffff);
    light.show();
    vTaskDelay(pdMS_TO_TICKS(2000));
    light.fill(0);
    light.show();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
  vTaskDelete(NULL);
}

#endif