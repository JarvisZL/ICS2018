#include <am.h>
#include <x86.h>
#include <amdev.h>

#define I8042_DATA_PORT 0x60
#define KEYDOWN_MASK 0x8000
int kb;

size_t input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      kb=inl(I8042_DATA_PORT);
      _KbdReg *kbd = (_KbdReg *)buf;
      kbd->keydown = (kb & KEYDOWN_MASK ? 1 : 0);
      kbd->keycode = kb&(~KEYDOWN_MASK);
      return sizeof(_KbdReg);
    }
  }
  return 0;
}
