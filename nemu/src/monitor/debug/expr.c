#include "nemu.h"
#include <stdlib.h>
#include <stdio.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NEQ,TK_LAND,TK_DEM,TK_HEX,TK_REG,TK_U32,TK_DERE
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
  {"!=", TK_NEQ},        //not equal
  {"&&", TK_LAND},       //logic and
  {"0[Xx][0-9a-fA-F]+",TK_HEX}, //16
  {"[0-9]+",TK_DEM},    //demical number
  {"\\$(eax|ebx|ecx|edx|esp|ebp|edi|esi|eip|ax|bx|cx|dx|sp|bp|si|di|al|ah|bl|bh|cl|ch|dl|dh)",TK_REG},//regs
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
	  case TK_NEQ: tokens[nr_token++].type=rules[i].token_type; break;
	  case TK_LAND: tokens[nr_token++].type=rules[i].token_type; break;
          case '+': tokens[nr_token++].type=rules[i].token_type; break;
	  case '-': tokens[nr_token++].type=rules[i].token_type; break;
	  case '*': tokens[nr_token++].type=rules[i].token_type; break;
          case '/': tokens[nr_token++].type=rules[i].token_type; break;
	  case '(': tokens[nr_token++].type=rules[i].token_type; break;
          case ')': tokens[nr_token++].type=rules[i].token_type; break;
	  case TK_HEX: tokens[nr_token].type=rules[i].token_type; strncpy(tokens[nr_token++].str,substr_start,substr_len); break;
	  case TK_REG:tokens[nr_token].type=rules[i].token_type; strncpy(tokens[nr_token++].str,substr_start,substr_len); break;
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

bool if_ze=false;//if there's a div 0 error!

uint32_t eval(int p,int q,bool* LE)
{

      bool legal=true;//whether the expression is legal	
      if(p>q)
      {
	printf("it's illegal!\n");
        *LE=false;
	return 0;
      }
      else if(p==q)
      {
          char* useless;
	  if(tokens[p].type==TK_DEM)
	   return (uint32_t) strtol(tokens[p].str,&useless,10);
	  else if(tokens[p].type==TK_HEX)
		  return (uint32_t) strtol(tokens[p].str,&useless,16);
	  else if(tokens[p].type==TK_REG)
	  {
		  if(!strcmp(tokens[p].str,"$eax"))
			  return  cpu.eax;
		  else if(!strcmp(tokens[p].str,"$ecx"))
			  return  cpu.ecx;
		  else if(!strcmp(tokens[p].str,"$edx"))
			  return  cpu.edx;
		  else if(!strcmp(tokens[p].str,"$ebx"))
			  return  cpu.ebx;
		  else if(!strcmp(tokens[p].str,"$esp"))
			  return cpu.esp;
		  else if(!strcmp(tokens[p].str,"$ebp"))
			  return  cpu.ebp;
		  else if(!strcmp(tokens[p].str,"$esi"))
			  return  cpu.esi;
		  else if(!strcmp(tokens[p].str,"$edi"))
			  return  cpu.edi;
		  else if(!strcmp(tokens[p].str,"$eip"))
			  return  cpu.eip;
		  
		  else if(!strcmp(tokens[p].str,"$ax"))
			  return  cpu.gpr[0]._16;
		  else if(!strcmp(tokens[p].str,"$cx"))
			  return  cpu.gpr[1]._16;
		  else if(!strcmp(tokens[p].str,"$dx"))
			  return  cpu.gpr[2]._16;
		  else if(!strcmp(tokens[p].str,"$bx"))
			  return  cpu.gpr[3]._16;
		  else if(!strcmp(tokens[p].str,"$sp"))
			  return  cpu.gpr[4]._16;
		  else if(!strcmp(tokens[p].str,"$bp"))
			  return  cpu.gpr[5]._16;
		  else if(!strcmp(tokens[p].str,"$si"))
			  return  cpu.gpr[6]._16;
		  else if(!strcmp(tokens[p].str,"$di"))
			  return  cpu.gpr[7]._16;

		  else if(!strcmp(tokens[p].str,"$al"))
			  return  cpu.gpr[0]._8[0];
		  else if(!strcmp(tokens[p].str,"$ah"))
			  return  cpu.gpr[0]._8[1];
		  else if(!strcmp(tokens[p].str,"$cl"))
			  return  cpu.gpr[1]._8[0];
		  else if(!strcmp(tokens[p].str,"$ch"))
			  return  cpu.gpr[1]._8[1];
		  else if(!strcmp(tokens[p].str,"$dl"))
			  return  cpu.gpr[2]._8[0];
		  else if(!strcmp(tokens[p].str,"$dh"))
			  return  cpu.gpr[2]._8[1];
		  else if(!strcmp(tokens[p].str,"$bl"))
			  return  cpu.gpr[3]._8[0];
		  else if(!strcmp(tokens[p].str,"$bh"))
			  return  cpu.gpr[3]._8[1];

		  else return -1;// actually, it's also impossible!!!
	  }
	  else return -1;//actually it's impossilble,but without it, gcc think it may not have return value.
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
                          uint32_t prio;//define priority about'==','!=','&&'(1), '+'(2) '-'(2) '*'(3) '/'(3)  dere(4) 
		  } O_P;
		 O_P op; 
                 op.posi=-1;
		 op.prio=100;
                  for(int i=p;i<=q;++i)
		  {
			  if(tokens[i].type!=TK_DERE && tokens[i].type!='+' && tokens[i].type!='-' && tokens[i].type!='*' && tokens[i].type!='/' && tokens[i].type!='(' && tokens[i].type!=')' && tokens[i].type!=TK_EQ && tokens[i].type!=TK_NEQ && tokens[i].type!=TK_LAND)
				  continue;
			  else
			  {
                                   if(tokens[i].type=='(')
					  par_cnt++;
				   else if(tokens[i].type==')')
					  par_cnt--;
				   //pars are the important preconditions
				   else if(tokens[i].type==TK_DERE)
				   {
					   if(par_cnt==0&&op.prio>=4)
					   {
						   op.posi=i;
						   op.prio=4;
					   }
					   else continue;
                                          /* if(i==p)
						return vaddr_read(eval(p+1,q,LE),4);
					   else continue;*/
				   }
				   else if(tokens[i].type==TK_EQ)
				   {
					   if(par_cnt==0&&op.prio>=0)
					   {
                                            op.posi=i;
					    op.prio=0;
					   }
					   else continue;
				   }
				   else if(tokens[i].type==TK_NEQ)
				   {
					   if(par_cnt==0&&op.prio>=0)
					   {
					   op.posi=i;
					   op.prio=0;
					   }
					   else continue;
				   }
				   else if(tokens[i].type==TK_LAND)
				   {
					   if(par_cnt==0&&op.prio>=1)
					   {
						   op.posi=i;
						   op.prio=1;
					   }
					   else continue;
				   }
				   else if(tokens[i].type=='+')
				  {
                                       if(par_cnt==0&&op.prio>=2)
				       {
					       op.posi=i;
					       op.prio=2;
				       }
				       else continue;
				  }
				   else if(tokens[i].type=='*')
				   {
					   if(par_cnt==0&&op.prio>=3)
					   {
						   op.posi=i;
						   op.prio=3;
					   }
					   else continue;
				   }
				   else if(tokens[i].type=='/')
				   {
					   if(par_cnt==0&&op.prio>=3)
					   {
						   op.posi=i;
						   op.prio=3;
					   }
                                           else continue;
				   }
				   else if(tokens[i].type=='-')
				   {
                                          if(i==p||(tokens[i-1].type!=TK_DEM&&tokens[i-1].type!=TK_HEX&&tokens[i-1].type!=')'))
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
									      if(j==q&&(tokens[j].type==TK_DEM||tokens[j].type==TK_HEX))
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
                                                  if(par_cnt==0&&op.prio>=2)
						  {
							  op.posi=i;
							  op.prio=2;
						  }
					  }

				   }
			  }
		  }

		  if(tokens[op.posi].type==TK_DERE)
		  {
			  return vaddr_read(eval(p+1,q,LE),4);
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
			  case TK_EQ:    return val1==val2; break;
		          case TK_NEQ:   return val1!=val2; break;
		          case TK_LAND:  return val1&&val2; break;	   
		          default: assert(0);
		  }	  
	     }

      }

}



uint32_t expr(char *e, bool *success) {
 // init_regex();
  memset(tokens,0,sizeof(tokens));
  if_ze=false;

  if (!make_token(e)) {
    *success = false;
    if(*success==false)
	    printf("make_token failed!\n");
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
//deal with deref and multy
 for(int i=0;i<nr_token;++i)
 {
	 if(tokens[i].type=='*' && (i==0 || tokens[i-1].type=='+' || tokens[i-1].type=='-' || tokens[i-1].type=='*' || tokens[i-1].type=='/' || tokens[i-1].type=='(' || tokens[i-1].type==TK_EQ || tokens[i-1].type==TK_NEQ || tokens[i-1].type==TK_LAND || tokens[i-1].type==TK_DERE))
		 tokens[i].type=TK_DERE;
 }
//deal over.
 uint32_t result;
 bool LEGAL=true;

 result=eval(0,nr_token-1,&LEGAL);
 *success=LEGAL; 

 if(if_ze==true)
	 printf("div zero error!\n");

 return result;
 //TODO();
}
