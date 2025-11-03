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
  volatile bool ntpSynced;
  char stationIP[16];
  void init() {
    this->ntpSynced = false;
    this->stationIP[15] = '\0';
    this->accessPointCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
    this->stationCredentialStatus = NetworkCredentialStatus::UNAVAILABLE;
  }
} NetworkInfo;


#endif