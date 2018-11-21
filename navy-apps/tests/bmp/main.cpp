#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>
int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("hello1\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  printf("hello2\n");
  NDL_Render();
  printf("hello3\n");
  NDL_CloseDisplay();
  printf("hello4\n");
  while (1);
  return 0;
}
