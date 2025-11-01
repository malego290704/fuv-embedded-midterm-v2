#ifndef _USER_REQUEST_H
#define _USER_REQUEST_H

#include <time.h>

#include "user.h"

typedef struct UserRequest {
  User* userP;
  struct tm timestamp;
  void init(User* userP) {
    this->userP = userP;
    if (!getLocalTime(&this->timestamp)) {
      // TODO: handle error
    }
  }
} UserRequest;
void initUserRequests(UserRequest *reqP, User *userP) {
  reqP->userP = userP;
  if (!getLocalTime(&reqP->timestamp)) {
    // TODO: handle error
    return;
  }
}

#endif