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
    Serial.println(format);
    LogMessage msg;
    va_list args;
    va_start(args, format);
    msg.init(level, format, args);
    va_end(args);
    xQueueSend(this->queueLogMessage, &msg, (TickType_t)0);
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
        xSemaphoreGive(this->serialMutex);
      }
    }
  }
} Logger;

#endif