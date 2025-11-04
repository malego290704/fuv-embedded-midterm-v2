#ifndef _WEBSERVER_H
#define _WEBSERVER_H

#include <LittleFS.h>

#include "webserverAPI.h"

#include "globalContext.h"

void webserverInit(GlobalContext* contextP) {
  AsyncWebServer* serverP = contextP->webserverP;
  Logger* loggerP = &contextP->logger;
  loggerP->log(LOGGER_DEBUG, "Init webserver!");
  serverP->on("/", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/index.html", "text/html");
  });
  serverP->on("/sign", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/sign.html", "text/html");
  });
  serverP->on("/an", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/an.html", "text/html");
  });
  serverP->on("/minh", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/minh.html", "text/html");
  });
  serverP->on("/guest", HTTP_GET, [](AsyncWebServerRequest* requestP) {
    requestP->send(LittleFS, "/web/guest.html", "text/html");
  });
  webserverAPIInit(contextP);
  loggerP->log(LOGGER_DEBUG, "Waiting for WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  loggerP->log(LOGGER_DEBUG, "Begin webserver");
  serverP->begin();
  loggerP->log(LOGGER_DEBUG, "Webserver Created!");
}

#endif