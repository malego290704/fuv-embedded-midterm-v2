#ifndef _LOGGER_HANDLER_H
#define _LOGGER_HANDLER_H

#include "logger/logger.h"

#include "globalContext.h"

void taskLoggerHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.sseP = contextP->loggerSSEP;
  contextP->logger.sseInitP = &contextP->loggerSSEInit;
  bool initSuccess = contextP->logger.init();
  if (!initSuccess) {
    vTaskDelete(NULL);
  };
  for (;;) {
    contextP->logger.resolve();
  }
  vTaskDelete(NULL);
}

#endif