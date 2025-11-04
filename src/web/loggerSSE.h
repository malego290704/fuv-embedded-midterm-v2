#ifndef _LOGGER_SSE_H
#define _LOGGER_SSE_H

#include "globalContext.h"

void webLoggerSSEInit(GlobalContext* contextP) {
  Logger* loggerP = &contextP->logger;
  loggerP->log(LOGGER_DEBUG, "Init Logger SSE!");
  // Reference: https://randomnerdtutorials.com/esp32-web-server-sent-events-sse/
  contextP->loggerSSEP->onConnect([loggerP](AsyncEventSourceClient *client){
    if (client->lastId()) {
      loggerP->log(LOGGER_DEBUG, "Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  contextP->webserverP->addHandler(contextP->loggerSSEP);
  contextP->loggerSSEInit = true;
}

#endif