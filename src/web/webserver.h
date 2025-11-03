#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <LittleFS.h>

#include "webserverAPI.h"

#include "globalContext.h"

void webserverInit(GlobalContext* contextP) {
  AsyncWebServer* serverP = contextP->webserverP;
  Logger logger = contextP->logger;
  logger.log(LOGGER_DEBUG, "Init webserver");
  serverP->on("/", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/index.html", "text/html");
  });
  serverP->on("/sign", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/sign.html", "text/html");
  });
  webserverAPIInit(contextP);
  logger.log(LOGGER_DEBUG, "Waiting for WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  logger.log(LOGGER_DEBUG, "Begin webserver");
  serverP->begin();
  logger.log(LOGGER_DEBUG, "Webserver Created!");
}

#endif