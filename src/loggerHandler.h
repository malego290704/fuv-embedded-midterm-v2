#ifndef _LOGGER_HANDLER_H
#define _LOGGER_HANDLER_H

#include "logger/logger.h"

#include "globalContext.h"

void taskLoggerHandler(void* pvParameters) {
  GlobalContext* context = (GlobalContext*)pvParameters;
  bool initSuccess = context->logger.init();
  if (!initSuccess) {
    vTaskDelete(NULL);
  };
  for (;;) {
    context->logger.resolve();
  }
  vTaskDelete(NULL);
}

#endif