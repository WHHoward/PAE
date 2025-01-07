#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);
uint32_t expr(char *e, bool *success);
int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);
/*  u_int32_t ans , result = 0;
  char str[65536];
  bool success = true;
  FILE *fp = fopen("tools/gen-expr/input","r");
  while(fscanf(fp,"%d %s",&ans,str) != 1 && fscanf(fp, "%[^\n]",str) != 1)
  {
    printf("ans = %u str = %s\n",ans,str);
    result = expr(str,&success);
    if(success == false)
    {
      printf("Bad expression\n");
    }
    else
    {
      if(result == ans)
      {
        printf("Correct\n");
      }
      else
      {
        printf("Wrong\n");
      }
    }
    result = 0;
    memset(str,0,sizeof(str));
  }
*/  
  return 0;
}
