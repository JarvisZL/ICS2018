#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i].str[0]='\0';
    wp_pool[i].ht=0;
  }
  wp_pool[NR_WP - 1].next = NULL;
  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* tem;

WP* new_wp()
{
	if(free_==NULL)
		assert(0);
	else 
	{
            tem=free_;
	    free_=free_->next;
	    tem->next=NULL;
	    if(head==NULL)
	    {
		    head=tem;
		    return tem;
	    }
	    else
	    {
		    tem->next=head;
		    head=tem;
		    return tem;
	    }
	}
}

WP* find_wp(char* s)
{
      if(head==NULL)
      {
            printf("no such a watchpoint!\n");
	    return head;
      }
      else
      {
           tem=head;
	   while(strcmp(tem->str,s)&&tem->next!=NULL)
	   {
		   tem=tem->next;
	   }
	   if(!strcmp(tem->str,s))
	   {
                  return tem;
	   }
	   else
	   {
                 return tem->next; 
	   }
      }
}

void free_wp(WP* wp)
{       
	if(wp==NULL)
		return;
	if(head==NULL)
	{
		printf("no using watchpoint!\n");
	}
        else if(head==wp)
        {
		if(wp->next==NULL)
		{
			head=NULL;
			wp->next=free_;
			free_=wp;
		}
		else
		{
                       head=head->next;
		       wp->next=free_;
		       free_=wp;
		}
	}
	else
	{
              tem=head;
	      while(tem->next!=wp&&tem->next!=NULL)
	      {
                      tem=tem->next;
		      
	      }
	      if(tem->next==wp)
	      {
		      tem->next=wp->next;
                      wp->next=free_;
		      free_=wp;
	      }
	      if(tem->next==NULL)
	      {
		      printf("the watchpoint isn't using!\n");
	      }
	}
}

bool checkwp()
{      
	bool all=false;
	bool succ;
	tem=head;
	while(tem!=NULL)
	{
           tem->new_v=expr(tem->str,&succ);
	   if(tem->new_v!=tem->old_v)
	   {
		   printf("watchpoint %d:  %s\n",tem->NO,tem->str);
		   printf("old value: %d\n",tem->old_v);
		   printf("new value: %d\n",tem->new_v);
		   printf("\n");
		   all=true;
		   tem->old_v=tem->new_v;
		   tem->ht++;
	   }
	  tem=tem->next; 
	}
	return all;
}

void wp_print()
{
     tem=head;
     while(tem!=NULL)
     {
	     printf("%d\twatchpoint\tkeep\ty\t      \t%s\n",tem->NO,tem->str);
	     printf("  \tbreakpoint already hit %d times\n",tem->ht);
	     tem=tem->next;
     }
}
