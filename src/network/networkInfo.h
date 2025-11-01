#ifndef _NETWORK_INFO_H
#define _NETWORK_INFO_H

#include "networkConfiguration.h"

typedef struct NetworkInfo {
  char accessPointSSID[WIFI_CREDENTIAL_MAX_LENGTH];
  char accessPointPassword[WIFI_CREDENTIAL_MAX_LENGTH];
  char stationSSID[WIFI_CREDENTIAL_MAX_LENGTH];
  char stationPassword[WIFI_CREDENTIAL_MAX_LENGTH];
} NetworkInfo;


#endif