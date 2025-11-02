#ifndef _NETWORK_OPERATION_H
#define _NETWORK_OPERATION_H

#include <WiFi.h>

#include "networkInfo.h"
#include "networkInfoOperation.h"

#include "globalContext.h"

void networkOperationInit(GlobalContext* context) {
  Logger logger = context->logger;
  logger.log(LOGGER_DEBUG, "Starting network operation");
  NetworkInfo* networkInfoP = &context->networkInfo;
  context->networkInfo.init();
  if (loadAccessPointCredentials(networkInfoP, context->littlefsMutex)) {
    logger.log(LOGGER_DEBUG, "Successfully read AP credentials!");
    logger.log(LOGGER_DEBUG, "AP SSID:     %s", networkInfoP->accessPointSSID);
    logger.log(LOGGER_DEBUG, "AP Password: %s", networkInfoP->accessPointPassword);
  }
  if (loadStationCredentials(networkInfoP, context->littlefsMutex)) {
    logger.log(LOGGER_DEBUG, "Successfully read STA credentials!");
    logger.log(LOGGER_DEBUG, "STA SSID:     %s", networkInfoP->stationSSID);
    logger.log(LOGGER_DEBUG, "STA Password: %s", networkInfoP->stationPassword);
  }
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(context->networkInfo.accessPointSSID, context->networkInfo.accessPointPassword);
  logger.log(LOGGER_DEBUG, "Successfully opened AP!");
}

void networkConnectWifiStation(GlobalContext* context) {
  Logger logger = context->logger;
  if (context->networkInfo.stationCredentialStatus != NetworkCredentialStatus::UNAVAILABLE) {
    logger.log(LOGGER_DEBUG, "Disconnecting WiFi...");
    WiFi.disconnect();
    WiFi.begin(context->networkInfo.stationSSID, context->networkInfo.stationPassword);
    for (uint8_t i = 0; i < 20; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(200));
    }
    if (WiFi.status() == WL_CONNECTED) {
      context->networkInfo.stationCredentialStatus = NetworkCredentialStatus::VALID;
      logger.log(LOGGER_DEBUG, "Successfully connected to WiFi!");
    }
  }
}

#endif