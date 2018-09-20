
#include <monitor/expr.h>

int init_monitor(int, char *[]);
void ui_mainloop(int);
void tst_cmd_p();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);
  
  tst_cmd_p();

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}


void tst_cmd_p(){
  // char old_re[50];
   unsigned int re;
   bool succ;
   char s[1000];
   FILE *fp=NULL;
   fp=fopen("/nemu/tools/gen-expr/input","r");
   while(!feof(fp))
   {
	 memset(s,0,sizeof(s));
	// fscanf(fp,"%s",old_re);
        // fgets(s,1000,fp);
       	 re=expr(s,&succ);
	 printf("%u\n",re);
   }
   fclose(fp);

}

