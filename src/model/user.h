#ifndef _USER_H
#define _USER_H

#include<cstdint>

#include"userPermission.h"

#include<configuration.h>

typedef struct User {
  char id[MAX_USER_ID_LENGTH];
  char name[MAX_USER_NAME_LENGTH];
  uint_fast32_t color;
  UserPermission permission;
  void init(const char *id, const char *name, const uint_fast32_t color, const UserPermission permission) {
    snprintf(this->id, MAX_USER_ID_LENGTH, "%s", id);
    snprintf(this->name, MAX_USER_NAME_LENGTH, "%s", name);
    this->color = color;
    this->permission = permission;
  }
} User;

#endif