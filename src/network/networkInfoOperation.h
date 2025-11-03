#ifndef _NETWORK_INFO_OPERATION_H
#define _NETWORK_INFO_OPERATION_H

#include <LittleFS.h>

#include "networkConfiguration.h"
#include "networkInfo.h"

bool loadAccessPointCredentials(NetworkInfo* networkInfoP, SemaphoreHandle_t fsMutex) {
  bool success;
  File file;
  size_t bytes_read;
  char ssid[WIFI_CREDENTIAL_MAX_LENGTH];
  char password[WIFI_CREDENTIAL_MAX_LENGTH];
  success = false;
  if (xSemaphoreTake(fsMutex, portMAX_DELAY) == pdTRUE) {
    file = LittleFS.open(ACCESS_POINT_SSID_FILENAME, FILE_READ);
    if (file && file.available()) {
      bytes_read = file.readBytes(ssid, WIFI_CREDENTIAL_MAX_LENGTH);
      ssid[bytes_read] = '\0';
      success = true;
    }
    if (file) {
      file.close();
    }
    xSemaphoreGive(fsMutex);
  }
  if (!success) {
    return false;
  }
  success = false;
  if (xSemaphoreTake(fsMutex, portMAX_DELAY) == pdTRUE) {
    file = LittleFS.open(ACCESS_POINT_PASSWORD_FILENAME, FILE_READ);
    if (file && file.available()) {
      bytes_read = file.readBytes(password, WIFI_CREDENTIAL_MAX_LENGTH);
      password[bytes_read] = '\0';
      success = true;
    }
    if (file) {
      file.close();
    }
    xSemaphoreGive(fsMutex);
  }
  if (!success) {
    return false;
  }
  snprintf(networkInfoP->accessPointSSID, WIFI_CREDENTIAL_MAX_LENGTH, "%s", ssid);
  snprintf(networkInfoP->accessPointPassword, WIFI_CREDENTIAL_MAX_LENGTH, "%s", password);
  networkInfoP->accessPointCredentialStatus = NetworkCredentialStatus::AVAILABLE;
  return true;
}

bool loadStationCredentials(NetworkInfo* networkInfoP, SemaphoreHandle_t fsMutex) {
  bool success;
  File file;
  size_t bytes_read;
  char ssid[WIFI_CREDENTIAL_MAX_LENGTH];
  char password[WIFI_CREDENTIAL_MAX_LENGTH];
  success = false;
  if (xSemaphoreTake(fsMutex, portMAX_DELAY) == pdTRUE) {
    file = LittleFS.open(STATION_SSID_FILENAME, FILE_READ);
    if (file && file.available()) {
      bytes_read = file.readBytes(ssid, WIFI_CREDENTIAL_MAX_LENGTH - 1);
      ssid[bytes_read] = '\0';
      success = true;
    }
    if (file) {
      file.close();
    }
    xSemaphoreGive(fsMutex);
  }
  if (!success) {
    return false;
  }
  success = false;
  if (xSemaphoreTake(fsMutex, portMAX_DELAY) == pdTRUE) {
    file = LittleFS.open(STATION_PASSWORD_FILENAME, FILE_READ);
    if (file && file.available()) {
      bytes_read = file.readBytes(password, WIFI_CREDENTIAL_MAX_LENGTH - 1);
      password[bytes_read] = '\0';
      success = true;
    }
    if (file) {
      file.close();
    }
    xSemaphoreGive(fsMutex);
  }
  if (!success) {
    return false;
  }
  snprintf(networkInfoP->stationSSID, WIFI_CREDENTIAL_MAX_LENGTH, "%s", ssid);
  snprintf(networkInfoP->stationPassword, WIFI_CREDENTIAL_MAX_LENGTH, "%s", password);
  networkInfoP->stationCredentialStatus = NetworkCredentialStatus::AVAILABLE;
  return true;
}

#endif