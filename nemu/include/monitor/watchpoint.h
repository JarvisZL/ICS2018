#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  char *str;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */


} WP;

WP* new_wp();
WP* find_wp(char*);
void free_wp(WP*);

#endif
