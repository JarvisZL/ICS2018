
#include "monitor/expr.h"


int init_monitor(int, char *[]);
void ui_mainloop(int);
void tst();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  
	
  int is_batch_mode = init_monitor(argc, argv);

//  tst();
  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}

void tst(){
   char c;
   int cnt;
   char s[200];
   FILE *fp=fopen("/home/jarviszly/ics2018/nemu/tools/gen-expr/input","r");
   if(fp==NULL)
   {
	   assert(0);
   } 
   while((c=fgetc(fp))!=EOF)
   {
	memset(s,0,sizeof(s));
	cnt=0;
        while(c!=' ')
	  c=fgetc(fp);
        
        c=fgetc(fp);

	while(c!='\n')
	{
            s[cnt++]=c;
	    c=fgetc(fp);
	}

	unsigned int re;
	bool succ = true;
	re=expr(s, &succ);
	printf("%u\n",re);
	re=0;

   }

   fclose(fp);
   return;
}

