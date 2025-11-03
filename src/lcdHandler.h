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
  LiquidCrystal_I2C lcd = *contextP->lcdP;
  DisplayState state = DisplayState::IDLE;
  lcd.init();
  lcd.clear();
  lcd.backlight();
  for (;;) {
    if (state == DisplayState::IDLE) {
      //
    } else if (state == DisplayState::USER_INFO) {
      //
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

#endif