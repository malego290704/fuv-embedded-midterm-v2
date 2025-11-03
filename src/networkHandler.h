#ifndef _NETWORK_HANDLER_H
#define _NETWORK_HANDLER_H

#include "network/networkOperation.h"

#include "globalContext.h"

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void taskNetworkHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskNetworkHandler");
  Logger* loggerP = &contextP->logger;
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname("ESP32SmartLockDevice");
  WiFiEventId_t wifiEventStationConnect = WiFi.onEvent([contextP](WiFiEvent_t event, WiFiEventInfo_t info) {
    contextP->logger.log(LOGGER_INFO, "Connected to WiFi as station!");
    networkTrySyncNTP(contextP);
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFiEventId_t wifiEventStationDisconnect = WiFi.onEvent([contextP](WiFiEvent_t event, WiFiEventInfo_t info) {
    contextP->networkInfo.stationIP[0] = '\0';
    contextP->logger.log(LOGGER_WARN, "Disconnected to WiFi as station!");
    contextP->logger.log(LOGGER_WARN, "Reason for disconnection: %s", info.wifi_sta_disconnected.reason);
    networkOperationInit(contextP);
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFiEventId_t wifiEventStationGotIP = WiFi.onEvent([contextP](WiFiEvent_t event, WiFiEventInfo_t info) {
    snprintf(contextP->networkInfo.stationIP, 16, "%s", WiFi.localIP().toString().c_str());
    contextP->logger.log(LOGGER_INFO, "Received IP as station!");
    contextP->logger.log(LOGGER_INFO, "IP: %s", contextP->networkInfo.stationIP);
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  networkConnectWifiStation(contextP);
  networkOperationInit(contextP);
  vTaskDelay(pdMS_TO_TICKS(5000));
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      loggerP->log(LOGGER_WARN, "Reconnecting to WiFi as station...");
      networkConnectWifiStation(contextP);
    }
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
  vTaskDelete(NULL);
}

#endif