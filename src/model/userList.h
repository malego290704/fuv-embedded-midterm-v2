#ifndef _USER_LIST_H
#define _USER_LIST_H

#include "user.h"

#include "configuration.h"

typedef struct UserList {
  uint32_t count = 0;
  User list[MAX_USER_COUNT];
  bool addUser(const char *id, const char *name, const uint_fast32_t color, const UserPermission permission) {
    if (count == MAX_USER_COUNT - 1) {
      return false;
    }
    this->list[this->count++].init(id, name, color, permission);
    return true;
  }
} UserList;


#endif