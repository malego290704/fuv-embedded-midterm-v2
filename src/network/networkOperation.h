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
  networkInfoP->init();
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
  WiFi.softAP(networkInfoP->accessPointSSID, networkInfoP->accessPointPassword);
}

void networkConnectWifiStation(GlobalContext* contextP) {
  Logger* loggerP = &contextP->logger;
  if (contextP->networkInfo.stationCredentialStatus != NetworkCredentialStatus::UNAVAILABLE) {
    WiFi.begin(contextP->networkInfo.stationSSID, contextP->networkInfo.stationPassword);
  }
}

void networkTrySyncNTP(GlobalContext* contextP) {
  if (contextP->networkInfo.ntpSynced) {
    return;
  }
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  Logger* loggerP = &contextP->logger;
  loggerP->log(LOGGER_DEBUG, "Syncing NTP...");
  configTime(25200, 0, "pool.ntp.org");
  if (time(NULL) > LOGGER_MIN_VALID_TIME) {
    loggerP->log(LOGGER_DEBUG, "NTP synced!");
    contextP->networkInfo.ntpSynced = true;
  } else {
    loggerP->log(LOGGER_WARN, "Cannot sync NTP!");
  }
}

#endif