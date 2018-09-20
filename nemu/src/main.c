
#include <monitor/expr.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);
void tst();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  tst();
	
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}

void tst(){
   char old_re[50];
   unsigned int re;
   bool succ;
   char s[200];
   FILE *fp=fopen("/home/jarviszly/ics2018/nemu/tools/gen-expr/input","r");
   if(fp==NULL)
   {
	   assert(0);
   } 
   while(!feof(fp))
   {
          memset(s,0,sizeof(s));
	 fscanf(fp,"%s",old_re);
         fgets(s,1000,fp);
       	 re=expr(s,&succ);
	 printf("%u\n",re);
   }
   fclose(fp);
   return;
}

