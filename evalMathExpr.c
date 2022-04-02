/* https://www.codewars.com/kata/52a78825cdfc2cfc87000005/ */

#include <assert.h>
#include <stdlib.h>
#include <math.h>

enum operators { VALUE, ADDITION, SUBTRACTION, MULTIPLICATION,
                    DIVISION, UNARY_MINUS };

typedef struct node {
  double value;
  enum operators operator;
  struct node *lNode;
  struct node *rNode;
} node;

node *nodeConstructor(double value, enum operators operator) {
  node *tmp = (node *) malloc(sizeof(node));
  if(tmp == NULL)
    return NULL;
  tmp->value = value;
  tmp->operator = operator;
  tmp->lNode = NULL;
  tmp->rNode = NULL;
  return tmp;
}

double strToDouble(const char **str) {
  double tmp = 0.;
  while(**str >= '0' && **str <= '9') {
    tmp = tmp * 10 + (**str - '0');
    ++(*str);
  }
  if(**str == '.') {
    int i = 0;
    ++(*str);
    while(**str >= '0' && **str <= '9') {
      tmp = tmp * 10 + (**str - '0');
      ++i;
      ++(*str);
    }
    tmp /= pow(10, i);
  }
  --(*str);
  return tmp;
}

node *createBinaryExpressionTree(const char **expression) {
  node *operator, *lOperand, *rOperand, *actRoot;
  operator = lOperand = rOperand = actRoot = NULL;
  while(**expression != '\0' && **expression != ')') {
    if(**expression == '(') {
      createBinaryExpressionTree(expression);
    } else if(**expression == '+') {
      operator = nodeConstructor(0., ADDITION);
      operator->lNode = lOperand;
      actRoot = operator;
    } else if(**expression == '-') {
      ;
    } else if(**expression == '*') {
      ;
    } else if(**expression == '/') {
      ;
    } else if(**expression >= '0' && **expression <= '9') {
      if(lOperand == NULL) {
        lOperand = nodeConstructor(strToDouble(expression), VALUE);
      } else {
        rOperand = nodeConstructor(strToDouble(expression), VALUE);
        operator->rNode = rOperand;
      }
    }
    ++(*expression);
  }
  return actRoot;
}

double evalTree(node *n) {
  if(n->operator == VALUE)
    return n->value;
  else if(n->operator == UNARY_MINUS)
    return (-1.0)*evalTree(n->rNode);
  double a, b;
  a = evalTree(n->lNode);
  b = evalTree(n->rNode);
  switch(n->operator) {
    case ADDITION:
      return a+b;
    case SUBTRACTION:
      return a-b;
    case MULTIPLICATION:
      return a*b;
    case DIVISION:
      return a/b;
  }
}

double calculate(const char *expression) {
  node *root = createBinaryExpressionTree(&expression);
  return evalTree(root);
}

/* TESTS */
int main(int argc, char *argv[]) {
/*
  char *expr[] = { "1+1", "1 - 1", "1* 1", "1 /1", "12*-1",
                   "-123", "123", "2 /2+3 * 4.75- -6", "12* 123",
                   "2 / (2 + 3) * 4.33 - -6" };
  double result[] = { 2., 0., 1., 1., -12., -123., 123., 21.25,
                      1476., 7.732 };
  for(int i = 0; i < 10; i++)
    assert(calculate(expr[i]) == result[i]);
*/
  assert(calculate("1+1") == 2.);
}