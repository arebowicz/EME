/* https://www.codewars.com/kata/52a78825cdfc2cfc87000005/ */
/* [UNFINISHED] */
/* TESTS PASSED: 68 */

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <stdio.h>

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
  node *operator, *actualRoot, *tmp, *parent;
  operator = actualRoot = tmp = parent = NULL;
  bool afterOperator, isParenthesis, isOperatorParenthesis;
  afterOperator = isParenthesis = false;
  while(**expression != '\0' && **expression != ')') {
    if(**expression == '(') {
      ++(*expression);
      tmp = createBinaryExpressionTree(expression);
      if(actualRoot == NULL) {
        actualRoot = tmp;
        if(tmp->lNode != NULL && tmp->rNode != NULL) {
          if(tmp->operator != UNARY_MINUS) {
            operator = actualRoot;
            isOperatorParenthesis = true;
          }
        }
      } else if(actualRoot->lNode == NULL && actualRoot->rNode == NULL)
        actualRoot->rNode = tmp;
      else if(operator->rNode != NULL /* && operator->rNode->operator == UNARY_MINUS */)
        operator->rNode->rNode = tmp;
      else
        operator->rNode = tmp;
      afterOperator = false;
      if(tmp->operator != VALUE && tmp->operator != UNARY_MINUS)
        isParenthesis = true;
  /* ^^ what about parent? ^^ */
    } else if(**expression == '+') {
      operator = nodeConstructor(0., ADDITION);
      operator->lNode = actualRoot;
      actualRoot = operator;
      parent = NULL;
      afterOperator = true;
      isParenthesis = false;
    } else if(**expression == '-') {
      if(actualRoot == NULL || afterOperator == true) {
        if(actualRoot == NULL)
          actualRoot = nodeConstructor(0., UNARY_MINUS);
        else
          operator->rNode = nodeConstructor(0., UNARY_MINUS);
        afterOperator = false;
      } else {
        operator = nodeConstructor(0., SUBTRACTION);
        operator->lNode = actualRoot;
        actualRoot = operator;
        parent = NULL;
        afterOperator = true;
      }
      isParenthesis = false;
    } else if(**expression == '*' || **expression == '/') {
      /* division is only "left"-associative operation:  (4/2)/2  =/=  4/(2/2) */
      /* and we have to "watch out" for expressions: 4/5/6/7 */
      /* also, in this algorithm: 3*4/5 == (3*4)/5 */
      if(operator == NULL) {
        operator = nodeConstructor(0., (**expression == '*') ? MULTIPLICATION : DIVISION);
        operator->lNode = actualRoot;
        actualRoot = operator;
      } else if(isParenthesis == true) {
        if(operator->operator == ADDITION
           || operator->operator == SUBTRACTION) {
          tmp = nodeConstructor(0., (**expression == '*') ? MULTIPLICATION : DIVISION);
          if(isOperatorParenthesis == true) {
            tmp->lNode = operator;
            actualRoot = operator = tmp;
            isOperatorParenthesis = false;
          } else {
            tmp->lNode = operator->rNode;
            operator->rNode = tmp;
            parent = operator;
            operator = tmp;
          }
        } else {
          tmp = nodeConstructor(0., (**expression == '*') ? MULTIPLICATION : DIVISION);
          tmp->lNode = operator;
          if(parent != NULL)
            parent->rNode = tmp;
          else
            actualRoot = tmp;
          operator = tmp;
        }
      } else if(operator->operator == ADDITION
                || operator->operator == SUBTRACTION) {
        tmp = operator;
        operator = nodeConstructor(0., (**expression == '*') ? MULTIPLICATION : DIVISION);
        operator->lNode = tmp->rNode;
        tmp->rNode = operator;
        parent = tmp;
      } else if(operator->operator == MULTIPLICATION
                || operator->operator == DIVISION) {
        tmp = nodeConstructor(0., (**expression == '*') ? MULTIPLICATION : DIVISION);
        tmp->lNode = operator;
        if(parent != NULL)
          parent->rNode = tmp;
        else if(actualRoot == operator)
          actualRoot = tmp;
        operator = tmp;
      }
      afterOperator = true;
      isParenthesis = false;
    } else if(**expression >= '0' && **expression <= '9') { /* get VALUE from expression */
      if(actualRoot == NULL) {    /* first value */
        actualRoot = nodeConstructor(strToDouble(expression), VALUE);
      } else {
        if(actualRoot->lNode == NULL && actualRoot->rNode == NULL) {  /* UNARY_MINUS */
          actualRoot->rNode = nodeConstructor(strToDouble(expression), VALUE);
        } else if(operator->rNode != NULL /* && operator->rNode->operator == UNARY_MINUS */)  /* UNARY_MINUS */
          operator->rNode->rNode = nodeConstructor(strToDouble(expression), VALUE);
        else
          operator->rNode = nodeConstructor(strToDouble(expression), VALUE);
      }
      afterOperator = false;
      isParenthesis = false;
    }
    ++(*expression);
  }
  return actualRoot;
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

void printTree(node *n) {
  if(n == NULL)
    return;
  switch(n->operator) {
    case VALUE:
      printf("%f\n", n->value);
      break;
    case ADDITION:
      printf("+\n");
      break;
    case SUBTRACTION:
      printf("-\n");
      break;
    case MULTIPLICATION:
      printf("*\n");
      break;
    case DIVISION:
      printf("/\n");
      break;
    case UNARY_MINUS:
      printf("- unary\n");
      break;
  }
  printTree(n->lNode);
  printTree(n->rNode);
}

double calculate(const char *expression) {
  node *root = createBinaryExpressionTree(&expression);

  printTree(root);

  return evalTree(root);
}

/* TESTS */
int main(int argc, char *argv[]) {
/*
  char *expr[] = { "1+1", "1 - 1", "1* 1", "1 /1", "12*-1", "-123",
                   "123.45", "2 /2+3 * 4.75- -6", "12* 123",
                   "2 / (2 + 3) * 4.33 - -6" };
  double result[] = { 2., 0., 1., 1., -12., -123., 123.45, 21.25,
                      1476., 7.732 };
  for(int i = 0; i < 10; i++)
    assert(calculate(expr[i]) == result[i]);
*/
  assert(calculate("(123.45*(678.90 / (-2.5+ 11.5)-(80 -19) *33.25) / 20 + 11)") == (123.45*(678.90 / (-2.5+ 11.5)-(80 -19) *33.25) / 20 + 11));
}