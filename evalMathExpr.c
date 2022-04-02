/* https://www.codewars.com/kata/52a78825cdfc2cfc87000005/ */

#include <stdbool.h>

enum operators { VALUE, PLUS, MINUS, MULTIPLICATION, DIVISION };

typedef struct node {
  double value;
  bool minus;
  enum operators operator;
  struct node *lNode;
  struct node *rNode;
} node;

double calculate(const char* expression) {
  return 0.;
}

/* TESTS */
int main(int argc, char *argv[]) {
  char *expr[] = { "12*-1", "1+1", "1 - 1", "1* 1", "1 /1",
                   "-123", "123", "2 /2+3 * 4.75- -6", "12* 123",
                   "2 / (2 + 3) * 4.33 - -6" };
  double result[] = { -12., 2., 0., 1., 1., -123., 123., 21.25,
                      1476., 7.732 };
  for(int i = 0; i < 10; i++)
    assert(calculate(expr[i]) == result[i]);
}