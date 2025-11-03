#ifndef _NETWORK_OPERATION_H
#define _NETWORK_OPERATION_H

#include <WiFi.h>

#include "networkInfo.h"
#include "networkInfoOperation.h"

#include "globalContext.h"

void networkOperationInit(GlobalContext* contextP) {
  Logger* loggerP = &contextP->logger;
  loggerP->log(LOGGER_DEBUG, "Starting network operation");
  NetworkInfo* networkInfoP = &contextP->networkInfo;
  contextP->networkInfo.init();
  if (loadAccessPointCredentials(networkInfoP, contextP->littlefsMutex)) {
    loggerP->log(LOGGER_DEBUG, "Successfully read AP credentials!");
    loggerP->log(LOGGER_DEBUG, "AP SSID:     %s", networkInfoP->accessPointSSID);
    loggerP->log(LOGGER_DEBUG, "AP Password: %s", networkInfoP->accessPointPassword);
  }
  if (loadStationCredentials(networkInfoP, contextP->littlefsMutex)) {
    loggerP->log(LOGGER_DEBUG, "Successfully read STA credentials!");
    loggerP->log(LOGGER_DEBUG, "STA SSID:     %s", networkInfoP->stationSSID);
    loggerP->log(LOGGER_DEBUG, "STA Password: %s", networkInfoP->stationPassword);
  }
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(contextP->networkInfo.accessPointSSID, contextP->networkInfo.accessPointPassword);
  loggerP->log(LOGGER_DEBUG, "Successfully opened AP!");
}

void networkConnectWifiStation(GlobalContext* contextP) {
  Logger* loggerP = &contextP->logger;
  if (contextP->networkInfo.stationCredentialStatus != NetworkCredentialStatus::UNAVAILABLE) {
    loggerP->log(LOGGER_DEBUG, "Disconnecting WiFi...");
    WiFi.disconnect();
    WiFi.begin(contextP->networkInfo.stationSSID, contextP->networkInfo.stationPassword);
    loggerP->log(LOGGER_DEBUG, "Connecting WiFi...");
    for (uint8_t i = 0; i < 20; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    if (WiFi.status() == WL_CONNECTED) {
      loggerP->log(LOGGER_DEBUG, "Successfully connected to WiFi!");
    }
  }
}

void networkSyncNTP(GlobalContext* contextP) {
  Logger* loggerP = &contextP->logger;
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  loggerP->log(LOGGER_DEBUG, "Syncing NTP...");
  configTime(25200, 0, "pool.ntp.org");
  loggerP->log(LOGGER_DEBUG, "NTP synced!");
}

#endif