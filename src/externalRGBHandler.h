#ifndef _EXTERNAL_RGB_HANDLER_H
#define _EXTERNAL_RGB_HANDLER_H

#include <Adafruit_NeoPixel.h>

#include "globalContext.h"

void taskExternalRGBHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskExternalRGBHandler");
  Logger* loggerP = &contextP->logger;
  Adafruit_NeoPixel* lightP = contextP->externalRGBP;
  loggerP->log(LOGGER_INFO, "Init External RGB");
  lightP->begin();
  lightP->fill(0);
  lightP->show();
  User* currentUser;
  uint32_t currentColor;
  for (;;) {
    if (xQueueReceive(contextP->externalRGBInfoQ, &currentUser, pdMS_TO_TICKS(10)) == pdTRUE) {
      loggerP->log(LOGGER_INFO, "Lighting ExternalRGB for %s", currentUser->name);
      currentColor = currentUser->color;
      if (currentUser->permission == UserPermission::Admin) {
        for (uint8_t i = 0; i < 5; i++) {
          lightP->fill(currentColor);
          lightP->show();
          vTaskDelay(pdMS_TO_TICKS(200));
          lightP->fill(0);
          lightP->show();
          vTaskDelay(pdMS_TO_TICKS(200));
        }
      } else if (currentUser->permission == UserPermission::Authorized) {
        for (uint8_t i = 0; i < 3; i++) {
          lightP->fill(currentColor);
          lightP->show();
          vTaskDelay(pdMS_TO_TICKS(300));
          lightP->fill(0);
          lightP->show();
          vTaskDelay(pdMS_TO_TICKS(200));
        }
      } else if (currentUser->permission == UserPermission::Unauthorized) {
        lightP->fill(currentColor);
        lightP->show();
        vTaskDelay(pdMS_TO_TICKS(1000));
        lightP->fill(0);
        lightP->show();
      }
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
  vTaskDelete(NULL);
}

#endif