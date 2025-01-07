#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void display_watchpoints()
{
  WP *p = head;
  printf("\033[0;34m");
  printf("Num\tExpr\tValue\n");
  while(p != NULL)
  {
    uint32_t num = p->val;
    int len = 0;
    while(num > 0)
    {
      num /= 10;
      len++;
    }
//    printf("%d\t%s\t%d\n",p->NO,p->expr,p->val);
    printf("%-16d%u(0x%-8x)%*s%s\n",p->NO,p->expr,p->val,20 - len," ",p->expr);
    p = p->next;
  }
  printf("\033[0m");
  return ;
}

WP* new_wp()
{
  if(free_ == NULL)
  {
    printf("\033[0;33m No more watchpoints can be set \033[0m\n");
    assert(0);
    return NULL;
  }
  WP *p = free_;
  free_ = free_->next;
  p->next = head;
  head = p;
  return p;
}

void free_wp(int n)
{
  if(head == NULL || n < 0 || n > 32)
  {
    printf("\033[0;33m No such watchpoint \033[0m\n");
    return ;
  }
  WP *p = head;
  WP *pre = NULL;
  while(p != NULL)
  {
    if(p->NO == n)
    {
      if(pre == NULL)
      {
        head = p->next;
      }
      else
      {
        pre->next = p->next;
      }
      p->next = free_;
      free_ = p;
      return ;
    }
    pre = p;
    p = p->next;
  }
  printf("\033[0;33m No such watchpoint \033[0m\n");
  return ;
}

bool check_wp()
{
  WP *p = head;
  bool success = true;
  while(p != NULL)
  {
    uint32_t val = expr(p->expr,&success);
    if(val != p->val)
    {
      printf("\033[0;33m Watchpoint %d: %s\n\033[0m",p->NO,p->expr);
      printf("\033[0;33m Old value = %u\n\033[0m",p->val);
      printf("\033[0;33m New value = %u\n\033[0m",val);
      p->val = val;
      return false;
    }
    p = p->next;
  }
  return true;
}