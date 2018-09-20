#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// this should be enough
static char buf[65536];
int buf_num=0;
int token_num=0;

uint32_t choose(uint32_t x)
{
      return (rand() %(x-0))+0; 
}

static void gen_num(){
     int lenth=(rand() %(50-1+1))+1;
     for(int i=0;i<lenth;++i)
     {
             buf[buf_num+i]=(rand() %(9-0+1))+0;
     }
     token_num++;

}

static inline void gen(char c){
	buf[buf_num++]=c;
	token_num++;
}

static inline void gen_rand_op(){
       int sel;
       sel=(rand() % (3-0+1))+0;
       switch(sel){
	       case 0: buf[buf_num++]='+'; break;
	       case 1: buf[buf_num++]='-'; break;
	       case 2: buf[buf_num++]='*'; break;
	       case 3: buf[buf_num++]='/'; break;
       } 
      token_num++;    
}

static  void gen_rand_space(){
      int sp_num;
      sp_num=(rand()%(5-0+1))+0;
      for(int i=0;i<sp_num;i++)
      {
	      buf[buf_num+i]=' ';
      }
      buf_num+=sp_num;
}


static inline void gen_rand_expr() {
         switch(choose(3)){
		 case 0: gen_rand_space(); gen_num(); gen_rand_space(); break;
                 case 1: gen_rand_space(); gen('('); gen_rand_expr(); gen(')'); gen_rand_space(); break;
	         default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
         }	
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
    while(token_num>100)
    {
	    token_num=0;
	    buf_num=0;
	    buf[0]='\0';
	    gen_rand_expr();
    }
    

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);

    token_num=0;
    buf_num=0;
    buf[0]='\0';
  }
  return 0;
}
