#ifndef _USER_REQUEST_HANDLER_H
#define _USER_REQUEST_HANDLER_H

#include "model/userRequest.h"

#include "configuration.h"
#include "globalContext.h"

void taskUserRequestHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskUserRequestHandler");
  contextP->userReqQ.init();
  UserRequest request;
  for (;;) {
    request = contextP->userReqQ.resolve();
  }
  vTaskDelete(NULL);
}

#endif