#ifndef _WEBSERVER_API_H
#define _WEBSERVER_API_H

#include <ArduinoJson.h>

#include "globalContext.h"

void apiUnlock(GlobalContext* contextP, AsyncWebServerRequest* request, uint8_t* data, size_t length) {
  Logger* loggerP = &contextP->logger;
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, data, length);
  if (error) {
    loggerP->log(LOGGER_ERROR, "deserializeJson() failed: %s", error.c_str());
    request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
    return;
  }
  if (doc["username"].is<const char*>()) {
    if (doc["data"].is<const char*>()) {
      loggerP->log(LOGGER_DEBUG, "Parsing Signature Request.");
      const char* requestData = doc["data"];
      const char* requestUsername = doc["username"];
      char prediction[MAX_USER_ID_LENGTH], username[MAX_USER_ID_LENGTH], response[128];
      float inputTensor[576];
      int j = 0;
      for (int i = 0; i < 576; i++) {
        if (requestData[j] == '\n') {
          j++;
        }
        inputTensor[i] = requestData[j++] - 48;
      }
      // loggerP->log(LOGGER_DEBUG, "Start Prediction...");
      snprintf(prediction, MAX_USER_ID_LENGTH, "%s", contextP->aiEngine.predict(inputTensor));
      snprintf(username, MAX_USER_ID_LENGTH, "%s", requestUsername);
      loggerP->log(LOGGER_DEBUG, "Got prediction: %s", prediction);
      snprintf(response, 128, "{\"status\":\"good\", \"prediction\":\"%s\"}", prediction);
      int c = contextP->users.count, userIndex = -1;
      // loggerP->log(LOGGER_DEBUG, "User list count: %d", c);
      for (int i = 0; i < c; i++) {
        // loggerP->log(LOGGER_DEBUG, "Comparing %s and %s", username, contextP->users.list[i].id);
        if (strcmp(contextP->users.list[i].id, username) == 0) {
          userIndex = i;
          break;
        }
      }
      loggerP->log(LOGGER_DEBUG, "Username should be %s at index %d", username, userIndex);
      for (int i = 0; i < c; i++) {
        // loggerP->log(LOGGER_DEBUG, "Comparing %s and %s", prediction, contextP->users.list[i].id);
        if (strcmp(contextP->users.list[i].id, prediction) == 0 && i == userIndex) {
          contextP->userReqQ.newRequest(&contextP->users.list[i]);
          loggerP->log(LOGGER_DEBUG, "User %s found", contextP->users.list[i].id);
          request->send(200, "application/json", response);
          return;
        }
      }
      request->send(400, "application/json", "{\"status\":\"good\", \"prediction\":\"guest\"}");
      return;
    }
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