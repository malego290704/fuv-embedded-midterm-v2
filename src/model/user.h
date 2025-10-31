#ifndef _USER_H
#define _USER_H

#include<cstdint>

#include"model/userPermission.h"

#include<configuration.h>

typedef struct User {
  char id[MAX_USER_ID_LENGTH];
  char name[MAX_USER_NAME_LENGTH];
  uint_fast32_t color;
  UserPermission permission;
} User;

#endif