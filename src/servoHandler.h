#ifndef _SERVO_HANDLER_H
#define _SERVO_HANDLER_H

#include "globalContext.h"

void taskServoHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskServoHandler");
  Logger* loggerP = &contextP->logger;
  Servo* servoP = contextP->servoP;
  servoP->write(0);
  User* currentUser;
  for (;;) {
    if (xQueueReceive(contextP->servoInfoQ, &currentUser, pdMS_TO_TICKS(10)) == pdTRUE) {
      loggerP->log(LOGGER_INFO, "Opening Servo for %s", currentUser->name);
      if (currentUser->permission == UserPermission::Admin) {
        servoP->write(90);
        vTaskDelay(pdMS_TO_TICKS(10000));
        servoP->write(0);
      } else if (currentUser->permission == UserPermission::Authorized) {
        servoP->write(90);
        vTaskDelay(pdMS_TO_TICKS(5000));
        servoP->write(0);
      } else if (currentUser->permission == UserPermission::Unauthorized) {
        for (int i = 0; i < 3; i++) {
          servoP->write(20);
          vTaskDelay(pdMS_TO_TICKS(500));
          servoP->write(0);
          vTaskDelay(pdMS_TO_TICKS(500));
        }
      }
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
}

#endif