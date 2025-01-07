#include "nemu.h"
#include <stdbool.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
uint32_t isa_reg_str2val(const char *s, bool *success);
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NOTEQ,TK_DECIMAL,TK_OR,TK_LESSEQ,TK_GREATEREQ,TK_LESS,TK_GREATER,TK_HEXADECIMAL,TK_AND,TK_REG,TK_DEFERENCE,TK_POSNUM,TK_NEGNUM

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
  {"==", TK_EQ},         // equal
  {"!=", TK_NOTEQ},         // not equal
  {"\\-", '-'},         // minus
  {"\\*", '*'},         // multiply
  {"\\/", '/'},         // divide
  {"\\(", '('},         // left bracket
  {"\\)", ')'},         // right bracket
  {"0[xX][0-9a-fA-F]+", TK_HEXADECIMAL},         // hexadecimal
  {"[0-9]+", TK_DECIMAL},         // decimal
  {"&&", TK_AND},         // and
  {"\\|\\|", TK_OR},         // or
  {"<=", TK_LESSEQ},         // less or equal
  {">=", TK_GREATEREQ},         // greater or equal
  {"<", TK_LESS},         // less
  {">", TK_GREATER},         // greater
  {"\\$(\\$0|ra|[sgt]p|t[0-6]|a[0-7]|s([0-9]|1[0-1]))", TK_REG}         // register
  
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

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
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

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

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE: break;
          case TK_DECIMAL:
          case TK_HEXADECIMAL:
          case TK_REG:
            tokens[nr_token].type = rules[i].token_type;
            if(substr_len >= 32) {
              printf("\033[0;33m too long token \033[0m\n");
              return false;
            }
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          default:
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
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
int op_precedence(int op) {
  switch(op) {
    case TK_NEGNUM:
    case TK_POSNUM: return 1;
    case TK_DEFERENCE: return 2;
    case '+':
    case '-':
      return 4;
    case '*':
    case '/':
      return 3;
    case TK_EQ:
    case TK_NOTEQ:
      return 7;
    case TK_AND:
      return 11;
    case TK_OR:
      return 12;
    case TK_LESSEQ:
    case TK_GREATEREQ:
    case TK_LESS:
    case TK_GREATER:
      return 6;
    default:
      return 0;
  }
  return 0;
}

uint32_t findmainop(int p,int q,bool *success)
{
  uint32_t op = p;
  int layer = 0;
  int precedence = 0; 
  for(int i = p; i <= q; i++)
  {
    if(layer == 0)
    {
      int type = tokens[i].type;
      if(type == '(')
      {
        layer++;
        continue;
      }
      else if(type == ')')
      {
        printf("\033[0;33m bad expression \033[0m\n");
        *success = false;
        return 0;
      }
      else
      {
        int type_precedence = op_precedence(type);
        if(type_precedence >= precedence)
        {
          precedence = type_precedence;
          op = i;
        }
      }
    }
    else
    {
      if(tokens[i].type == '(')
      {
        layer++;
      }
      else if(tokens[i].type == ')')
      {
        layer--;
      }
    }
  }
  if(layer != 0 || precedence == 0)
  {
    printf("\033[0;33m bad expression at [%d %d]\033[0m\n",p,q);
    *success = false;
    return 0;
  }
  return op;
}
uint32_t calc(int pos, bool *success)
{
  uint32_t val = 0;
  uint32_t i = 0;
  if(tokens[pos].type != TK_REG)
  {
    bool isDeci = tokens[pos].type == TK_DECIMAL ? true : false;
    while(tokens[pos].str[i] != '\0')
    {
      if(isDeci) val = val * 10 + tokens[pos].str[i] - '0';
      else
      {
        if(i >= 10)
        {
          printf("\033[0;33m too long hexadecimal number \033[0m\n");
          *success = false;
          return 0;
        }
          if(i == 0 || i == 1)
          {
            i++;
            continue;
          }
          val = val * 16;
          if(tokens[pos].str[i] >= '0' && tokens[pos].str[i] <= '9') val += tokens[pos].str[i] - '0';
          else if(tokens[pos].str[i] >= 'a' && tokens[pos].str[i] <= 'f') val += tokens[pos].str[i] - 'a' + 10;
          else if(tokens[pos].str[i] >= 'A' && tokens[pos].str[i] <= 'F') val += tokens[pos].str[i] - 'A' + 10;      
        }
        i++;
    }
  }
  else
  {
    val = isa_reg_str2val(tokens[pos].str + 1, success);
  }
  return val;
}

int check_parentheses(int p,int q)
{
  int layer = 0;
  int res = -1;
  if(tokens[p].type == '(' && tokens[q].type == ')')
  {
    res = 1;
    for(int i = p + 1; i < q; i++)
    {
      if(tokens[i].type == '(') layer++;
      else if(tokens[i].type == ')') layer--;
      if(layer < 0)
      {
        res = -1;
        break;
      }
    }
  }
  layer = 0;
  for(int i = p; i <= q; i++)
  {
    if(layer < 0)
    {
      res = 0;
      break;
    }
    if(tokens[i].type == '(') layer++;
    else if(tokens[i].type == ')') layer--;
  }
  if(layer != 0) return 0;
  return res;
}

uint32_t eval(int p,int q,bool *success)
{
  if(p > q)
  {
    printf("\033[0;33m bad expression at [%d %d]\033[0m\n",p,q);
    *success = false;
    return 0;
  }
  else if(p == q)
  {
    if(tokens[p].type != TK_DECIMAL && tokens[p].type != TK_HEXADECIMAL && tokens[p].type != TK_REG)
    {
      printf("\033[0;33m bad expression at [%d %d]\033[0m\n",p,q);
      *success = false;
      return 0;
    }
    else
    {
      return calc(p,success);
    }
  }
  int check_p = check_parentheses(p,q);
  if(check_p != -1)
  {
    if(check_p == 0)
    {
      printf("Bad expression at [%d %d] for !check_parentheses\n",p,q);
      *success = false;
      return 0;
    }
    return eval(p + 1,q - 1,success);
  }
  else
  {
    int op = findmainop(p,q,success);
    if(*success == false) return 0;
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    if(tokens[op].type == TK_NOTEQ || tokens[op].type == TK_EQ || tokens[op].type == TK_AND || tokens[op].type == TK_OR || tokens[op].type == TK_LESSEQ || tokens[op].type == TK_GREATEREQ || tokens[op].type == TK_LESS || tokens[op].type == TK_GREATER)
    {
      val1 = eval(p,op - 1,success);
      val2 = eval(op + 1,q,success);
    }
    else
    {
      val1 = eval(p,op - 1,success);
      val2 = eval(op + 1,q,success);
    }
    if(*success == false) return 0;
    switch(tokens[op].type)
    {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': //return val1 / val2;
        if(val2 == 0)
        {
          printf("\033[0;33m bad expression at [%d %d]\033[0m\n",p,q);
          *success = false;
          return 0;
        }
        return val1 / val2;
      case TK_EQ: return val1 == val2;
      case TK_NOTEQ: return val1 != val2;
      case TK_AND: return val1 && val2;
      case TK_OR: return val1 || val2;
      case TK_LESSEQ: return val1 <= val2;
      case TK_GREATEREQ: return val1 >= val2;
      case TK_LESS: return val1 < val2;
      case TK_GREATER: return val1 > val2;
      case TK_DEFERENCE: return vaddr_read(val2,4);
      case TK_POSNUM: return val2;
      case TK_NEGNUM: return -val2;
      default:
        printf("\033[0;33m bad expression at [%d %d]\033[0m\n",p,q);
        *success = false;
        return;
    }
  }
  return 0;
}
uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  //TODO();
  for(int i = 0; i < nr_token; i++)
  {
    if(tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != TK_DECIMAL && tokens[i - 1].type != TK_HEXADECIMAL && tokens[i - 1].type != TK_REG && tokens[i - 1].type != ')')))
    {
      tokens[i].type = TK_DEFERENCE;
    }
    if(tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != TK_DECIMAL && tokens[i - 1].type != TK_HEXADECIMAL && tokens[i - 1].type != TK_REG && tokens[i - 1].type != ')')))
    {
      tokens[i].type = TK_NEGNUM;
    }
    if(tokens[i].type == '+' && (i == 0 || (tokens[i - 1].type != TK_DECIMAL && tokens[i - 1].type != TK_HEXADECIMAL && tokens[i - 1].type != TK_REG && tokens[i - 1].type != ')')))
    {
      tokens[i].type = TK_POSNUM;
    }
  }
  *success = true;
  return eval(0,nr_token - 1,success);
//  return 0;
}
