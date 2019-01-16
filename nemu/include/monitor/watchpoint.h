#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char str[200];//cannot use pointers as the things in this addrs maybe change
  struct watchpoint *next;
  uint32_t old_v;
  uint32_t new_v;
  uint32_t ht;
  /* TODO: Add more members if necessary */


} WP;

//WP* new_wp();
//WP* find_wp(char*);
//void free_wp(WP*);

#endif
