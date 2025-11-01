#include <Arduino.h>

#include "model/user.h"
#include "model/userPermission.h"
#include "model/userRequest.h"
#include "model/userList.h"


#include "onboardRGBHandler.h"
#include "networkHandler.h"


#include "configuration.h"
#include "globalContext.h"
struct GlobalContext context;


#include<Adafruit_NeoPixel.h>
Adafruit_NeoPixel onboardRGB(1, 45, NEO_GRB + NEO_KHZ800);


void initUsers() {
  UserList users = context.users;
  users.addUser("guest", "Guest", 0xFF0000, UserPermission::Unauthorized);
  users.addUser("minh", "Minh", 0x00FFFF, UserPermission::Admin);
  users.addUser("an", "An", 0xFF00FF, UserPermission::Authorized);
}


void initGlobalContext() {
  initUsers();
  context.onboardRGB = onboardRGB;
  context.littlefsMutex = xSemaphoreCreateMutex();
  xSemaphoreTake(context.littlefsMutex, portMAX_DELAY);
  LittleFS.begin();
  xSemaphoreGive(context.littlefsMutex);
}


void setup() {
  Serial.begin(115200);
  initGlobalContext();
  delay(5000);
  Serial.println("Hello World!");
  xTaskCreate(taskOnboardRGBHandler, "OnboardRGBHandler", 2048, &context, 1, NULL);
  xTaskCreate(taskNetworkHandler, "NetworkHandler", 8192, &context, 1, NULL);
  Serial.println("Finish init!");
}

void loop() {
  // vTaskDelete(NULL);
  delay(1);
}