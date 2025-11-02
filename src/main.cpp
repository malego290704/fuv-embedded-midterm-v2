#include <Arduino.h>

#include "model/user.h"
#include "model/userPermission.h"
#include "model/userRequest.h"
#include "model/userList.h"


#include "onboardRGBHandler.h"
#include "networkHandler.h"
#include "loggerHandler.h"


#include "configuration.h"
#include "globalContext.h"
struct GlobalContext context;


#define LOG_E(format, ...) context.logger.log(LOGGER_ERROR, format, ##__VA_ARGS__)
#define LOG_W(format, ...) context.logger.log(LOGGER_WARN,  format, ##__VA_ARGS__)
#define LOG_I(format, ...) context.logger.log(LOGGER_INFO,  format, ##__VA_ARGS__)
#define LOG_D(format, ...) context.logger.log(LOGGER_DEBUG, format, ##__VA_ARGS__)


#include<Adafruit_NeoPixel.h>
Adafruit_NeoPixel onboardRGB(1, 45, NEO_GRB + NEO_KHZ800);


void initUsers() {
  UserList users = context.users;
  users.addUser("guest", "Guest", 0xFF0000, UserPermission::Unauthorized);
  users.addUser("minh", "Minh", 0x00FFFF, UserPermission::Admin);
  users.addUser("an", "An", 0xFF00FF, UserPermission::Authorized);
}
void initFS() {
  xSemaphoreTake(context.littlefsMutex, portMAX_DELAY);
  LittleFS.begin();
  xSemaphoreGive(context.littlefsMutex);
}


void initGlobalContext() {
  initUsers();
  context.onboardRGB = onboardRGB;
  context.networkInfo.init();
  context.littlefsMutex = xSemaphoreCreateMutex();
  initFS();
}


void setup() {
  Serial.begin(115200);
  initGlobalContext();
  delay(5000);
  xTaskCreate(taskLoggerHandler, "LoggerHandler", 32000U, &context, 0, NULL);
  Serial.println("Init Logger");
  while (!context.logger.initSuccess) {
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  LOG_I("Finished init logger");
  xTaskCreate(taskOnboardRGBHandler, "OnboardRGBHandler", 2048, &context, 1, NULL);
  xTaskCreate(taskNetworkHandler, "NetworkHandler", 8192, &context, 1, NULL);
  LOG_I("Finished setup()");
}

void loop() {
  // vTaskDelete(NULL);
  delay(1);
}