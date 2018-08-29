#ifndef _DEV_H_
#define _DEV_H_

#include <stdint.h>

typedef struct device {
  int device_id;                           // The Device ID
  char device_name[30];
} task_t;


#endif