#ifndef _NETWORK_INFO_H
#define _NETWORK_INFO_H

#include "networkConfiguration.h"

enum NetworkCredentialStatus {
  UNAVAILABLE,
  INVALID,
  VALID
};

typedef struct NetworkInfo {
  char accessPointSSID[WIFI_CREDENTIAL_MAX_LENGTH];
  char accessPointPassword[WIFI_CREDENTIAL_MAX_LENGTH];
  NetworkCredentialStatus accessPointCredentialStatus;
  char stationSSID[WIFI_CREDENTIAL_MAX_LENGTH];
  char stationPassword[WIFI_CREDENTIAL_MAX_LENGTH];
  NetworkCredentialStatus stationCredentialStatus;
  void init() {
    this->accessPointCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
    this->stationCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
  }
} NetworkInfo;


#endif