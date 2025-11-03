#ifndef _WEBSERVER_API_H
#define _WEBSERVER_API_H

#include <ArduinoJson.h>

#include "globalContext.h"

void apiUnlock(GlobalContext* contextP, AsyncWebServerRequest* request, uint8_t* data, size_t length) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, data, length);
  if (error) {
    contextP->logger.log(LOGGER_ERROR, "deserializeJson() failed: %s", error.c_str());
    request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    return;
  }
  if (doc["data"].is<const char*>()) {
    const char* requestData = doc["data"];
    contextP->logger.log(LOGGER_DEBUG, requestData);
    request->send(200, "application/json", "{\"status\":\"good\"}");
    return;
  }
  request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON template\"}");
}

void webserverAPIInit(GlobalContext* contextP) {
  AsyncWebServer* serverP = contextP->webserverP;
  Logger* loggerP = &contextP->logger;
  serverP->on("/api/unlock", HTTP_POST, [](AsyncWebServerRequest* request){
    //
  }, NULL, [contextP](AsyncWebServerRequest* request, uint8_t* data, size_t length, size_t index, size_t total){
    apiUnlock(contextP, request, data, length);
  });
}

#endif