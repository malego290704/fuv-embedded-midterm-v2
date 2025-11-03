#ifndef _NETWORK_INFO_H
#define _NETWORK_INFO_H

#include "networkConfiguration.h"

enum NetworkCredentialStatus {
  UNAVAILABLE,
  AVAILABLE
};

typedef struct NetworkInfo {
  char accessPointSSID[WIFI_CREDENTIAL_MAX_LENGTH + 1];
  char accessPointPassword[WIFI_CREDENTIAL_MAX_LENGTH + 1];
  NetworkCredentialStatus accessPointCredentialStatus;
  char stationSSID[WIFI_CREDENTIAL_MAX_LENGTH + 1];
  char stationPassword[WIFI_CREDENTIAL_MAX_LENGTH + 1];
  NetworkCredentialStatus stationCredentialStatus;
  void init() {
    this->accessPointCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
    this->stationCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
  }
} NetworkInfo;


#endif