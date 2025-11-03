#ifndef _NETWORK_HANDLER_H
#define _NETWORK_HANDLER_H

#include "network/networkOperation.h"

#include "globalContext.h"

void taskNetworkHandler(void* pvParameters) {
  GlobalContext* contextP = (GlobalContext*)pvParameters;
  contextP->logger.log(LOGGER_INFO, "Started taskNetworkHandler");
  networkOperationInit(contextP);
  networkConnectWifiStation(contextP);
  networkSyncNTP(contextP);
  for (;;) {
    // Serial.println("Network Infinite Loop!");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}

#endif