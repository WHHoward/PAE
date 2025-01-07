#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void display_watchpoints();
void isa_reg_display();
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
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
static int cmd_exp(char *args);
static int cmd_scan(char *args);
//set watchpoint
static int cmd_setWatchPoints(char *args);
//delete watchpoint
static int cmd_delWatchPoints(char *args);
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single step execution", cmd_si },
  { "info", "Print the information of registers or watchpoints", cmd_info },
  { "x", "Scan the memory", cmd_scan },
  { "p", "Expression evaluation", cmd_exp },
  { "w", "Set watchpoints", cmd_setWatchPoints },
  { "d", "Delete watchpoints", cmd_delWatchPoints },
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
static int cmd_si(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL) {
    cpu_exec(1);
  }
  else {
    int n = 0;
    for(int i = 0; i < strlen(args); i++) {
      n = n * 10 + arg[i];
    }
    cpu_exec(n);
  }
  return 0;
}
static int cmd_info(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL) {
    printf("Please input the argument\n");
    return 0;
  }
  if(strcmp(arg, "r") == 0) {
    isa_reg_display();
  }
  else if(strcmp(arg, "w") == 0) {
    display_watchpoints();
  }
  else {
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
static int cmd_exp(char *args) {
  if(args == NULL) {
    printf("Please input the expression\n");
    return 0;
  }
  bool success = true;
  uint32_t res = expr(args, &success);
  if(success) {
    printf("\033[0;32m %s = %d(%#x)\033[0m\n", args, res, res);
  }
  return 0;
}
static int cmd_scan(char *args) {
  bool success = true;
  uint32_t n = expr(args, &success);
  if(!success) {
    printf("Invalid expression\n");
    return 0;
  }
  printf("\033[0;32m %s = %d(%#x)\033[0m\n", args, n, n);
}
static int cmd_setWatchPoints(char *args) {
  if(args == NULL) {
    printf("Please input the expression\n");
    return 0;
  }
  bool success = true;
  printf("args = %s\n", args);
  uint32_t res = expr(args, &success);
  if(success) {
    WP *wp = new_wp();
    wp->val = res;
    printf("Set watchpoint %d at %s\n", wp->NO, args);
  }
  return 0;
}
static int cmd_delWatchPoints(char *args) {
  char *arg = strtok(NULL, " ");
  if(arg == NULL) {
    printf("Please input the watchpoint number\n");
    return 0;
  }
  int n = 0;
  bool success = true;
  n = expr(arg, &success);
  if(!success) {
    printf("Invalid expression\n");
    return 0;
  }
  free_wp(n);
  return 0;
}
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
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
