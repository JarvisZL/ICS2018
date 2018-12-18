#include <am.h>

void _trm_init() {
}

void _putc(char ch) {
}

void _halt(int code) {
  __asm__ volatile(".byte 0xd6": :"a"(code));
    while(1);
}

_Area _heap;
