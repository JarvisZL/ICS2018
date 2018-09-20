
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
   char c;
   int cnt=0;
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
        do{
	  c=fgetc(fp);
	}while(c!=' ');
        
        do{
	  c=fgetc(fp);
	  s[cnt++]=c;
	}while(c!='\n');
         
	re=expr(s,&succ);
	printf("%u\n",re);

   }
   fclose(fp);
   return;
}

