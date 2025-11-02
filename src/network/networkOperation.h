#ifndef _NETWORK_OPERATION_H
#define _NETWORK_OPERATION_H

#include <WiFi.h>

#include "networkInfo.h"
#include "networkInfoOperation.h"

#include "globalContext.h"

void networkOperationInit(GlobalContext* context) {
  if (loadAccessPointCredentials(&context->networkInfo, context->littlefsMutex)) {
    Serial.println("Successfully read AP credentials!");
    Serial.println(context->networkInfo.accessPointSSID);
    Serial.println(context->networkInfo.accessPointPassword);
  }
  if (loadStationCredentials(&context->networkInfo, context->littlefsMutex)) {
    Serial.println("Successfully read STA credentials!");
    Serial.println(context->networkInfo.stationSSID);
    Serial.println(context->networkInfo.stationPassword);
  }
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(context->networkInfo.accessPointSSID, context->networkInfo.accessPointPassword);
}

void networkConnectWifiStation(GlobalContext* context) {
  if (context->networkInfo.stationCredentialStatus != NetworkCredentialStatus::UNAVAILABLE) {
    WiFi.disconnect();
    WiFi.begin(context->networkInfo.stationSSID, context->networkInfo.stationPassword);
    for (uint8_t i = 0; i < 5; i++) {
      if (WiFi.status() == WL_CONNECTED) {
        break;
      }
    }
    if (WiFi.status() == WL_CONNECTED) {
      context->networkInfo.stationCredentialStatus = NetworkCredentialStatus::VALID;
      Serial.println("Successfully connected to WiFi!");
    }
  }
}

#endif