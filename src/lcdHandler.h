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
  lcdP->noBacklight();
  vTaskDelay(pdMS_TO_TICKS(2000));
  lcdP->backlight();
  contextP->logger.log(LOGGER_INFO, "Starting taskLCDHandler loop");
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
      //
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

#endif