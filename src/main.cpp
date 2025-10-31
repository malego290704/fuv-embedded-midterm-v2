#include <Arduino.h>







#include "model/user.h"
#include "model/userPermission.h"
#include "model/userRequest.h"









#include "configuration.h"




User users[MAX_USER_COUNT];
uint32_t userCount = 0;
void setUserData(const char *id, const char *name, const uint_fast32_t color, const UserPermission permission) {
  User *user = &users[userCount++];
  snprintf(user->id, MAX_USER_ID_LENGTH, "%s", id);
  snprintf(user->name, MAX_USER_NAME_LENGTH, "%s", name);
  user->color = color;
  user->permission = permission;
  return;
}
void initUsers() {
  setUserData("guest", "Guest", 0xFF0000, UserPermission::Unauthorized);
  setUserData("minh", "Minh", 0x00FFFF, UserPermission::Admin);
  setUserData("an", "An", 0xFF00FF, UserPermission::Authorized);
}




QueueHandle_t queueUserIndexAccessRequest;
QueueHandle_t queueNeoHandler;
void initQueues() {
  queueUserIndexAccessRequest = xQueueCreate(MAX_ACCESS_QUEUE_LENGTH, sizeof(UserRequest));
  queueNeoHandler = xQueueCreate(MAX_ACCESS_QUEUE_LENGTH, sizeof(User));
}


void taskUserIndexAccessRequest(void*) {
  UserRequest currentRequest;
  for (;;) {
    if (xQueueReceive(queueUserIndexAccessRequest, &currentRequest, portMAX_DELAY)) {
      xQueueSend(queueNeoHandler, &currentRequest.user, 0);
    }
  }
}



#include<Adafruit_NeoPixel.h>
Adafruit_NeoPixel onboardRGB(1, 45, NEO_GRB + NEO_KHZ800);
void taskNeoHandler(void*) {
  onboardRGB.begin();
  onboardRGB.fill(onboardRGB.Color(0,0,0));
  onboardRGB.show();
  User currentUser;
  uint32_t currentColor;
  for (;;) {
    if (xQueueReceive(queueNeoHandler, &currentUser, portMAX_DELAY)) {
      currentColor = currentUser.color;
      if (currentUser.permission == UserPermission::Admin) {
        for (uint8_t i = 0; i < 5; i++) {
          onboardRGB.fill(currentColor);
          onboardRGB.show();
          vTaskDelay(200 / portTICK_PERIOD_MS);
          onboardRGB.fill(0);
          onboardRGB.show();
          vTaskDelay(200 / portTICK_PERIOD_MS);
        }
      } else if (currentUser.permission == UserPermission::Authorized) {
        for (uint8_t i = 0; i < 3; i++) {
          onboardRGB.fill(currentColor);
          onboardRGB.show();
          vTaskDelay(300 / portTICK_PERIOD_MS);
          onboardRGB.fill(0);
          onboardRGB.show();
          vTaskDelay(200 / portTICK_PERIOD_MS);
        }
      } else if (currentUser.permission == UserPermission::Unauthorized) {
        onboardRGB.fill(currentColor);
        onboardRGB.show();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        onboardRGB.fill(0);
        onboardRGB.show();
      }
      vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
  }
}







void setup() {
  initUsers();
  initQueues();
  xTaskCreate(taskUserIndexAccessRequest, "UserRequestHandler", 2048, NULL, 1, NULL);
  xTaskCreate(taskNeoHandler, "RGBHandler", 2048, NULL, 0, NULL);
  delay(3000);
  xQueueSend(queueUserIndexAccessRequest, &users[1], portMAX_DELAY);
}

void loop() {
  vTaskDelete(NULL);
}