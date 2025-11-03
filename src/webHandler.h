#ifndef _WEB_HANDLER_H
#define _WEB_HANDLER_H

#include "web/webserver.h"

#include "globalContext.h"

void taskWebHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskWebHandler");
  webserverInit(contextP);
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
  vTaskDelete(NULL);
}

#endif