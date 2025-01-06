#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define NR_TOKENS 32
// this should be enough
static char buf[65536];
static int nr_tokens = 0;
static int strSub = 0;

static inline uint32_t choose(uint32_t n) {
  return rand() % n;
}
static inline void gen_num()
{
  uint32_t num = rand() % 1000;
  char str[32];
  snprintf(str,32,"%u",num);
  int len = strlen(str);
  strcpy(buf + strSub,str);
  strSub += len;
  nr_tokens++;
  return;
}
static inline void gen(char c)
{
  buf[strSub++] = c;
  nr_tokens++;
  return;
}
static inline void gen_rand_op()
{
  switch(choose(4))
  {
    case 0: gen('+'); break;
    case 1: gen('-'); break;
    case 2: gen('*'); break;
    case 3: gen('/'); break;
  }
  strSub++;
  nr_tokens++;
  return;
}
static inline void gen_rand_expr() {
//  buf[0] = '\0';
  switch (choose(3))
  {
    case 0:
      if(nr_tokens + 1 >= NR_TOKENS)
      {
        break;
      }
      gen_num();
      break;
    case 1:
      if(nr_tokens + 3 >= NR_TOKENS)
      {
        break;
      }
      gen('(');
      gen_rand_expr();
      //gen_rand_op();
      //gen_rand_expr();
      gen(')');
      break;
    default:
      if(nr_tokens + 3 >= NR_TOKENS)
      {
        break;
      }
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
  }
  switch (choose(2))
  {
    case 1:
      if(nr_tokens + 1 >= NR_TOKENS)
      {
        break;
      }
      buf[strSub++] = ' ';
      nr_tokens++;
      break;
    default:
      break;
  }
  return;
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
    strSub = 0;
    nr_tokens = 0;
    memset(buf,0,sizeof(buf));
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
