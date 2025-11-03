#ifndef _LOGGER_H
#define _LOGGER_H

#include <cstdint>
#include <semphr.h>
#include <HardwareSerial.h>
#include <time.h>

#include "logLevel.h"
#include "logMessage.h"

#include "configuration.h"

typedef struct Logger {
  uint32_t countReceived = 0, countPrinted = 0;
  SemaphoreHandle_t serialMutex;
  QueueHandle_t queueLogMessage;
  bool initSuccess = false;
  bool init() {
    this->serialMutex = xSemaphoreCreateMutex();
    this->queueLogMessage = xQueueCreate(LOG_QUEUE_MAX_LENGTH, sizeof(LogMessage));
    this->initSuccess = true;
    return true;
  }
  void log(LogLevel level, const char *format, ...) {
    if (!this->initSuccess) return;
    LogMessage msg;
    msg.timestamp = time(NULL);
    msg.level = level;
    va_list args;
    va_start(args, format);
    int l = vsnprintf(msg.message, LOG_MESSAGE_MAX_LENGTH, format, args);
    va_end(args);
    if (l >= LOG_MESSAGE_MAX_LENGTH) {
      msg.message[LOG_MESSAGE_MAX_LENGTH - 2] = '.';
      msg.message[LOG_MESSAGE_MAX_LENGTH - 3] = '.';
      msg.message[LOG_MESSAGE_MAX_LENGTH - 4] = '.';
    }
    xQueueSend(this->queueLogMessage, &msg, pdMS_TO_TICKS(10));
    delay(20);
  }
  void logNow(const char *msg) {
    if (xSemaphoreTake(this->serialMutex, portMAX_DELAY) == pdTRUE) {
      Serial.println(msg);
      delay(30);
      xSemaphoreGive(this->serialMutex);
    }
  }
  void resolve() {
    LogMessage msg;
    if (xQueueReceive(this->queueLogMessage, &msg, portMAX_DELAY) == pdTRUE) {
      char timeString[24];
      if (msg.timestamp < LOGGER_MIN_VALID_TIME) {
        long seconds = (long)msg.timestamp;
        long minutes = seconds / 60;
        long hours = minutes / 60;
        seconds %= 60;
        minutes %= 60;
        sprintf(timeString, "[UPTIME %05lu:%02lu:%02lu]", hours, minutes, seconds);
      } else {
        struct tm timeinfo;
        localtime_r(&msg.timestamp, &timeinfo);
        strftime(timeString, sizeof(timeString), "[%Y-%m-%d %H:%M:%S]", &timeinfo);
      }
      const char* level_str;
      switch(msg.level) {
        case LOGGER_ERROR: level_str = " [ERROR] "; break;
        case LOGGER_WARN:  level_str = " [WARN]  "; break;
        case LOGGER_INFO:  level_str = " [INFO]  "; break;
        case LOGGER_DEBUG: level_str = " [DEBUG] "; break;
        default:           level_str = " [UNKN]  "; break;
      }
      if (xSemaphoreTake(this->serialMutex, portMAX_DELAY) == pdTRUE) {
        Serial.print(timeString);
        Serial.print(level_str);
        Serial.println(msg.message);
        delay(200);
        xSemaphoreGive(this->serialMutex);
      }
    }
  }
} Logger;

#endif