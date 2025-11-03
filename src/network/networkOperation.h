#ifndef _NETWORK_OPERATION_H
#define _NETWORK_OPERATION_H

#include <WiFi.h>

#include "networkInfo.h"
#include "networkInfoOperation.h"

#include "globalContext.h"

void networkOperationInit(GlobalContext* contextP) {
  Logger logger = contextP->logger;
  logger.log(LOGGER_DEBUG, "Starting network operation");
  NetworkInfo* networkInfoP = &contextP->networkInfo;
  contextP->networkInfo.init();
  if (loadAccessPointCredentials(networkInfoP, contextP->littlefsMutex)) {
    logger.log(LOGGER_DEBUG, "Successfully read AP credentials!");
    logger.log(LOGGER_DEBUG, "AP SSID:     %s", networkInfoP->accessPointSSID);
    logger.log(LOGGER_DEBUG, "AP Password: %s", networkInfoP->accessPointPassword);
  }
  if (loadStationCredentials(networkInfoP, contextP->littlefsMutex)) {
    logger.log(LOGGER_DEBUG, "Successfully read STA credentials!");
    logger.log(LOGGER_DEBUG, "STA SSID:     %s", networkInfoP->stationSSID);
    logger.log(LOGGER_DEBUG, "STA Password: %s", networkInfoP->stationPassword);
  }
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(contextP->networkInfo.accessPointSSID, contextP->networkInfo.accessPointPassword);
  logger.log(LOGGER_DEBUG, "Successfully opened AP!");
}

void networkConnectWifiStation(GlobalContext* contextP) {
  Logger logger = contextP->logger;
  if (contextP->networkInfo.stationCredentialStatus != NetworkCredentialStatus::UNAVAILABLE) {
    logger.log(LOGGER_DEBUG, "Disconnecting WiFi...");
    WiFi.disconnect();
    WiFi.begin(contextP->networkInfo.stationSSID, contextP->networkInfo.stationPassword);
    for (uint8_t i = 0; i < 20; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    if (WiFi.status() == WL_CONNECTED) {
      logger.log(LOGGER_DEBUG, "Successfully connected to WiFi!");
    }
  }
}

void networkSyncNTP(GlobalContext* contextP) {
  Logger logger = contextP->logger;
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  logger.log(LOGGER_DEBUG, "Syncing NTP...");
  configTime(25200, 0, "pool.ntp.org");
  logger.log(LOGGER_DEBUG, "NTP synced!");
}

#endif