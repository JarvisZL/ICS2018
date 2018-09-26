#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char str[200];
  struct watchpoint *next;
  uint32_t old_v;
  uint32_t new_v;
  /* TODO: Add more members if necessary */


} WP;

//WP* new_wp();
//WP* find_wp(char*);
//void free_wp(WP*);

#endif
