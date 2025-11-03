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
  contextP->logger.log(LOGGER_INFO, "Starting taskLCDHandler loop");
  for (;;) {
    if (state == DisplayState::IDLE) {
      lcd.setCursor(0, 0);
      if (contextP->networkInfo.accessPointIP[0] == '\0') {
        lcd.print("No IP for AP    ");
      } else {
        lcd.print("A               ");
        lcd.setCursor(1, 0);
        lcd.print(contextP->networkInfo.accessPointIP);
      }
      lcd.setCursor(0, 1);
      if (contextP->networkInfo.stationIP[0] == '\0') {
        lcd.print("No IP for STA   ");
      } else {
        lcd.print("S               ");
        lcd.setCursor(1, 1);
        lcd.print(contextP->networkInfo.stationIP);
      }
    } else if (state == DisplayState::USER_INFO) {
      //
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
  vTaskDelete(NULL);
}

#endif