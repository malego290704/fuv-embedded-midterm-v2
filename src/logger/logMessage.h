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
} LogMessage;


#endif