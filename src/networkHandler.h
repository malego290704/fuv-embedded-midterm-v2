#ifndef _NETWORK_HANDLER_H
#define _NETWORK_HANDLER_H

#include "network/networkOperation.h"

#include "globalContext.h"

void taskNetworkHandler(void* pvParameters) {
  GlobalContext* context = (GlobalContext*)pvParameters;
  networkOperationInit(context);
  networkConnectWifiStation(context);
  for (;;) {
    // Serial.println("Network Infinite Loop!");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  vTaskDelete(NULL);
}

#endif