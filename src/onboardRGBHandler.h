#ifndef _ONBOARD_RGB_HANDLER_H
#define _ONBOARD_RGB_HANDLER_H

#include <Adafruit_NeoPixel.h>

#include "globalContext.h"

void taskOnboardRGBHandler(void* pvParameters) {
  GlobalContext* context = (GlobalContext*)pvParameters;
  Logger* loggerP = &context->logger;
  Adafruit_NeoPixel light = context->onboardRGB;
  loggerP->log(LOGGER_INFO, "Init Onboard RGB");
  light.begin();
  for (;;) {
    // Serial.println("Onboard RGB Infinite Loop!");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}

#endif