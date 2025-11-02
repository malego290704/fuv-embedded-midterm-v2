#ifndef _LOG_MESSAGE_H
#define _LOG_MESSAGE_H

#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "logLevel.h"

#include "configuration.h"

typedef struct LogMessage {
  time_t timestamp;
  LogLevel level;
  char message[LOG_MESSAGE_MAX_LENGTH];
  void init(LogLevel level, const char* format, ...) {
    this->timestamp = time(NULL);
    this->level = level;
    va_list args;
    va_start(args, format);
    int l = vsnprintf(this->message, LOG_MESSAGE_MAX_LENGTH, format, args);
    if (l >= LOG_MESSAGE_MAX_LENGTH) {
      this->message[LOG_MESSAGE_MAX_LENGTH - 2] = '.';
      this->message[LOG_MESSAGE_MAX_LENGTH - 3] = '.';
      this->message[LOG_MESSAGE_MAX_LENGTH - 4] = '.';
    }
    va_end(args);
  }
} LogMessage;


#endif