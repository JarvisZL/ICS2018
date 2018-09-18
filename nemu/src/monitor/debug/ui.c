#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_p(char *args);

static int cmd_x(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Step Through", cmd_si},
  { "info", "print program status", cmd_info}, 
  { "p", "print express", cmd_p},
  { "x", "scan memory", cmd_x},


  
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;
 
  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
  

static int cmd_si(char *args){
  int num;
  if(args==NULL) 
     num=1;
  else 
    {
	num=atoi(args);
    }  
  cpu_exec(num);
  return 0;
};
	
static int cmd_info(char *args){
     const char s[2]="r";
    if(strcmp(args,s)==0)
    {
      printf("eax\t 0x%x \t%d\n",cpu.eax,cpu.eax);
      printf("ecx\t 0x%x \t%d\n",cpu.ecx,cpu.ecx);     
      printf("edx\t 0x%x \t%d\n",cpu.edx,cpu.edx);
      printf("ebx\t 0x%x \t%d\n",cpu.ebx,cpu.ebx);
      printf("esp\t 0x%x \t%d\n",cpu.esp,cpu.esp);
      printf("ebp\t 0x%x \t%d\n",cpu.ebp,cpu.ebp);
      printf("esi\t 0x%x \t%d\n",cpu.esi,cpu.esi);
      printf("ebi\t 0x%x \t%d\n",cpu.edi,cpu.edi);
      printf("eip\t 0x%x \t%d\n",cpu.eip,cpu.eip);
       
      printf("ax\t 0x%x \t%d\n",cpu.gpr[0]._16,cpu.gpr[0]._16);
      printf("cx\t 0x%x \t%d\n",cpu.gpr[1]._16,cpu.gpr[1]._16);
      printf("dx\t 0x%x \t%d\n",cpu.gpr[2]._16,cpu.gpr[2]._16);
      printf("bx\t 0x%x \t%d\n",cpu.gpr[3]._16,cpu.gpr[2]._16);
      printf("sp\t 0x%x \t%d\n",cpu.gpr[4]._16,cpu.gpr[4]._16);
      printf("bp\t 0x%x \t%d\n",cpu.gpr[5]._16,cpu.gpr[5]._16);
      printf("si\t 0x%x \t%d\n",cpu.gpr[6]._16,cpu.gpr[6]._16);
      printf("di\t 0x%x \t%d\n",cpu.gpr[7]._16,cpu.gpr[7]._16);
      
      printf("al\t 0x%x \t%d\n",cpu.gpr[0]._8[0],cpu.gpr[0]._8[0]);
      printf("cl\t 0x%x \t%d\n",cpu.gpr[1]._8[0],cpu.gpr[1]._8[0]);
      printf("dl\t 0x%x \t%d\n",cpu.gpr[2]._8[0],cpu.gpr[2]._8[0]);
      printf("bl\t 0x%x \t%d\n",cpu.gpr[3]._8[0],cpu.gpr[3]._8[0]);
       
      printf("ah\t 0x%x \t%d\n",cpu.gpr[0]._8[1],cpu.gpr[0]._8[1]);
      printf("ch\t 0x%x \t%d\n",cpu.gpr[1]._8[1],cpu.gpr[1]._8[1]); 
      printf("dh\t 0x%x \t%d\n",cpu.gpr[2]._8[1],cpu.gpr[2]._8[1]);
      printf("bh\t 0x%x \t%d\n",cpu.gpr[3]._8[1],cpu.gpr[3]._8[1]);
      
    }
return 0;
};
//now the cmd_p is not completed

static int cmd_p(char *args){
       char* aft_num;
       long num=0;
       static long cnt=0;
       char* tst;
       char* _cmd;
       tst=strtok(args," ");
       if(strcmp(tst,args)!=0)
       {
	   _cmd=tst+strlen(tst)+1;
	   num=strtol(_cmd,&aft_num,16);
	   return (int) num;
       }
       else
       {
	   _cmd=args;
           num=strtol(_cmd,&aft_num,16);
	   cnt++;
	   printf("$%ld = %ld\n",cnt,num);
	   return (int) num;
       }
}

static int cmd_x(char *args){
        char* s1=strtok(args," ");
	int len=atoi(s1);
	char* s2=args;
	int addr=cmd_p(s2);
        for(int i=0;i<len;++i)
	{
          printf("0x%x: 0x%x\n",addr+i*4,vaddr_read(addr+i*4,4));
	}
  return 0;
}




void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  while (1) {
    char *str = rl_gets();
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
