#ifndef _USER_REQUEST_QUEUE_H
#define _USER_REQUEST_QUEUE_H

#include <queue.h>

#include "userRequest.h"

#include "configuration.h"

typedef struct UserRequestQueue {
  QueueHandle_t queue;
  void init() {
    this->queue = xQueueCreate(MAX_ACCESS_QUEUE_LENGTH, sizeof(UserRequest));
  }
  void newRequest(User* userP) {
    UserRequest request;
    request.init(userP);
    xQueueSend(this->queue, &request, pdMS_TO_TICKS(20));
  }
  UserRequest resolve() {
    UserRequest request;
    if (xQueueReceive(this->queue, &request, portMAX_DELAY) == pdTRUE) {
      return request;
    }
    return request;
  }
} UserRequestQueue;

#endif