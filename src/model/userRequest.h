#ifndef _USER_REQUEST_H
#define _USER_REQUEST_H

#include <time.h>

#include "user.h"

typedef struct UserRequest {
  User* userP;
  time_t timestamp;
  void init(User* userP) {
    this->userP = userP;
    this->timestamp = time(NULL);
  }
} UserRequest;
void initUserRequests(UserRequest *reqP, User *userP) {
  reqP->userP = userP;
  reqP->timestamp = time(NULL);
}

#endif