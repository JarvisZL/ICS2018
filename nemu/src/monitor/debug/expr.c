#include "nemu.h"
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_DEM,TK_U32

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},        // sub
  {"\\*", '*'},        //mult
  {"\\/", '/'},        //div
  {"\\(", '('},        //'('
  {"\\)", ')'},        //')'
  {"==", TK_EQ},        // equal
  {"[0-9]+",TK_DEM},    //demical number
  {"[Uu]", TK_U32}     // uint32_t
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[50];
} Token;

Token tokens[150];
int nr_token;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

//       Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
  //      i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
	  case TK_NOTYPE: break;
	  case TK_DEM: tokens[nr_token].type=rules[i].token_type; strncpy(tokens[nr_token++].str,substr_start,substr_len); break;
	  case TK_EQ: tokens[nr_token++].type=rules[i].token_type; break;
	  case TK_U32: break;
          case '+': tokens[nr_token++].type=rules[i].token_type; break;
	  case '-': tokens[nr_token++].type=rules[i].token_type; break;
	  case '*': tokens[nr_token++].type=rules[i].token_type; break;
          case '/': tokens[nr_token++].type=rules[i].token_type; break;
	  case '(': tokens[nr_token++].type=rules[i].token_type; break;
          case ')': tokens[nr_token++].type=rules[i].token_type; break;
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(int p,int q,bool* le)
{
     bool ok=true;
     int stack=0;
     for(int i=p;i<=q;++i)
     {
         if(tokens[i].type=='(') 
	      stack++;
	 else if(tokens[i].type==')')
	 {
		if(stack==0)
		{
			ok=false;
			*le=false;
			break;
		}
		else
		{
			stack--;
		}
	 }
	 else
	 {
	         if(stack==0)
	          {
			  ok=false;
		  }
	 }   
     }
     if(stack!=0)
     {
	     ok=false;
	     *le=false;
     }

     return ok;
}

bool if_ze=false;

uint32_t eval(int p,int q,bool* LE)
{

      bool legal=true;	
      if(p>q)
      {
	printf("it's illegal!\n");
        *LE=false;
	return 0;
      }
      else if(p==q)
      {
          char* useless;
	  return (uint32_t) strtol(tokens[p].str,&useless,10);
      }
      else if(check_parentheses(p,q,&legal)==true)
      {
	      return eval(p+1,q-1,LE);
      }
      else
      {
             if(legal==false)
	     {
	       *LE=false;
               printf("the expression is illegal!\n");
	       return 0; 
	     }
	     else
	     {
		  uint32_t par_cnt=0;//let it know whether operater is in pars
                 typedef struct OP{
			  int posi;
                          uint32_t prio;//define priority about '+'(1) '-'(1) '*'(2) '/'(2)
		  } O_P;
		 O_P op; 
                 op.posi=-1;
		 op.prio=100;
                  for(int i=p;i<=q;++i)
		  {
			  if(tokens[i].type!='+'&&tokens[i].type!='-'&&tokens[i].type!='*'&&tokens[i].type!='/'&&tokens[i].type!='('&&tokens[i].type!=')')
				  continue;
			  else
			  {
                                   if(tokens[i].type=='(')
					  par_cnt++;
				   else if(tokens[i].type==')')
					  par_cnt--;
				   else if(tokens[i].type=='+')
				  {
                                       if(par_cnt==0&&op.prio>=1)
				       {
					       op.posi=i;
					       op.prio=1;
				       }
				       else continue;
				  }
				   else if(tokens[i].type=='*')
				   {
					   if(par_cnt==0&&op.prio>=2)
					   {
						   op.posi=i;
						   op.prio=2;
					   }
					   else continue;
				   }
				   else if(tokens[i].type=='/')
				   {
					   if(par_cnt==0&&op.prio>=2)
					   {
						   op.posi=i;
						   op.prio=2;
					   }
                                           else continue;
				   }
				   else if(tokens[i].type=='-')
				   {
                                          if(i==p||(tokens[i-1].type!=TK_DEM&&tokens[i-1].type!='('&&tokens[i-1].type!=')'))
                                               {
						       if(i==p)
						       {
							      int cnt=1;
							      for(int j=p+1;j<=q;j++)
							      {
								      if(tokens[j].type=='-')
									      cnt++;
								      else
								      {
									      if(j==q&&tokens[j].type==TK_DEM)
									      {
										      if(cnt%2==0) return eval(q,q,LE);
									              else return 0-eval(q,q,LE);
									      }
								              else break; 	      
								      }
							      }

						       }
					       }
					  else
					  {
                                                  if(par_cnt==0&&op.prio>=1)
						  {
							  op.posi=i;
							  op.prio=1;
						  }
					  }

				   }
			  }
		  }
                  
	          int  val1=eval(p,op.posi-1,LE);
	          int  val2=eval(op.posi+1,q,LE);
	          switch(tokens[op.posi].type)
		  {
			  case '+': return val1+val2;
			  case '-': return val1-val2;
			  case '*': return val1*val2;
			  case '/': 
				    {
					    if(val2==0)
					    {
						    *LE=false;
						    if_ze=true;
						    return 0;
					    }
					    else return val1/val2;
					    break;
				    }
		          default: assert(0);
		  }	  
	     }

      }

}



uint32_t expr(char *e, bool *success) {
  memset(tokens,0,sizeof(tokens));
  if_ze=false;

  if (!make_token(e)) {
    *success = false;
    if(*success==false)
	    printf("make_token failed!");
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
 uint32_t result;
 bool LEGAL=true;

 result=eval(0,nr_token-1,&LEGAL);
 *success=LEGAL; 

 if(if_ze==true)
	 printf("div zero error!\n");

 return result;
 //TODO();
}
