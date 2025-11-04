#ifndef _LCD_HANDLER_H
#define _LCD_HANDLER_H

#include "globalContext.h"

enum class DisplayState {
  IDLE,
  USER_INFO
};

void taskLCDHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskLCDHandler");
  LiquidCrystal_I2C* lcdP = contextP->lcdP;
  DisplayState state = DisplayState::IDLE;
  lcdP->clear();
  User* currentUser;
  for (;;) {
    if (state == DisplayState::IDLE) {
      lcdP->setCursor(0, 0);
      if (contextP->networkInfo.accessPointIP[0] == '\0') {
        lcdP->print("No IP for AP    ");
      } else {
        lcdP->print("A               ");
        lcdP->setCursor(1, 0);
        lcdP->print(contextP->networkInfo.accessPointIP);
      }
      lcdP->setCursor(0, 1);
      if (contextP->networkInfo.stationIP[0] == '\0') {
        lcdP->print("No IP for STA   ");
      } else {
        lcdP->print("S               ");
        lcdP->setCursor(1, 1);
        lcdP->print(contextP->networkInfo.stationIP);
      }
    } else if (state == DisplayState::USER_INFO) {
      lcdP->clear();
      lcdP->setCursor(0, 0);
      lcdP->printf("Hello %s", currentUser->name);
      lcdP->setCursor(0, 1);
      if (currentUser->permission == UserPermission::Admin) {
        lcdP->print("ADMIN");
      } else if (currentUser->permission == UserPermission::Authorized) {
        lcdP->print("AUTHENTICATED");
      } else {
        lcdP->print("UNAUTHENTICATED");
      }
      state = DisplayState::IDLE;
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    if (xQueueReceive(contextP->lcdInfoQ, &currentUser, pdMS_TO_TICKS(1000)) == pdTRUE) {
      state = DisplayState::USER_INFO;
    }
  }
  vTaskDelete(NULL);
}

#endif