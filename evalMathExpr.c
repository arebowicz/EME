/* https://www.codewars.com/kata/52a78825cdfc2cfc87000005/ */

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
  node *operator, *actRoot, *tmp, *tmpDivision, *tmpMultiplication;
  operator = actRoot = tmp = tmpDivision = tmpMultiplication = NULL;
  bool afterOperator = false;
  while(**expression != '\0' && **expression != ')') {
    if(**expression == '(') {
      ++(*expression);
      tmp = createBinaryExpressionTree(expression);
      if(actRoot == NULL) {
        actRoot = tmp;
        if(tmp->lNode != NULL && tmp->rNode != NULL) {
          if(tmp->operator != UNARY_MINUS) {
            operator = actRoot;
          }
        }
      } else if(actRoot->lNode == NULL && actRoot->rNode == NULL) {
        actRoot->rNode = tmp;
      } else if(operator->rNode != NULL /* && operator->rNode->operator == UNARY_MINUS */)
        operator->rNode->rNode = tmp;
      else
        operator->rNode = tmp;
      afterOperator = false;
  /* ^^ what about tmpDivision and tmpMultiplication? ^^ */
    } else if(**expression == '+') {
      operator = nodeConstructor(0., ADDITION);
      operator->lNode = actRoot;
      actRoot = operator;
      tmpDivision = NULL;
      tmpMultiplication = NULL;
      afterOperator = true;
    } else if(**expression == '-') {
      if(actRoot == NULL || afterOperator == true) {
        if(actRoot == NULL)
          actRoot = nodeConstructor(0., UNARY_MINUS);
        else
          operator->rNode = nodeConstructor(0., UNARY_MINUS);
      } else {
        operator = nodeConstructor(0., SUBTRACTION);
        operator->lNode = actRoot;
        actRoot = operator;
        tmpDivision = NULL;
        tmpMultiplication = NULL;
        afterOperator = true;
      }
    } else if(**expression == '*') {
      if(operator == NULL) {
        operator = nodeConstructor(0., MULTIPLICATION);
        operator->lNode = actRoot;
        actRoot = operator;
  /* ^^ what about tmpDivision and tmpMultiplication? ^^ */
      } else if(operator->operator == DIVISION) {
        tmp = nodeConstructor(0., MULTIPLICATION);
        tmp->lNode = operator;
        if(tmpMultiplication == NULL)
          actRoot = tmp;
        else
          tmpMultiplication->rNode = tmp;
        operator = tmp;
      } else {
        tmp = operator;
        operator = nodeConstructor(0., MULTIPLICATION);
        operator->lNode = tmp->rNode;
        tmp->rNode = operator;
        tmpDivision = NULL;
        tmpMultiplication = operator;
      }
      afterOperator = true;
    } else if(**expression == '/') {
      /* division is only "left"-associative operation:  (4/2)/2  =/=  4/(2/2) */
      /* and we have to "watch out" for expressions: 4/5/6/7 */
      /* also, in this algorithm: 3*4/5 == 3*(4/5) */
      if(operator == NULL) {
        operator = nodeConstructor(0., DIVISION);
        operator->lNode = actRoot;
        actRoot = operator;
  /* ^^ what about tmpDivision and tmpMultiplication? ^^ */
      } else if(operator->operator == MULTIPLICATION) {  /* when the last operation was '*' */
        tmp = nodeConstructor(0., DIVISION);
        tmp->lNode = operator->rNode;
        operator->rNode = tmp;
        tmpDivision = operator;
        tmpMultiplication = operator;
        operator = tmp;
      } else if(operator->operator == DIVISION) {
        if(tmpDivision != NULL) {    /* when "penultimate" operation was '*'  */
          tmp = nodeConstructor(0., DIVISION);
          tmp->lNode = operator;
          tmpDivision->rNode = tmp;
          operator = tmp;
        } else {          /* when "penultimate" operation was '+' or '-'  */
          tmp = nodeConstructor(0., DIVISION);
          tmp->lNode = operator;
          actRoot->rNode = tmp;
          tmpMultiplication = actRoot;
          operator = tmp;
        }
      } else {
        tmp = nodeConstructor(0., DIVISION);
        tmp->lNode = operator->rNode;
        operator->rNode = tmp;
        tmpMultiplication = operator;
        operator = tmp;
      }
      afterOperator = true;
    } else if(**expression >= '0' && **expression <= '9') {
      if(actRoot == NULL) {
        actRoot = nodeConstructor(strToDouble(expression), VALUE);
      } else {
        if(actRoot->lNode == NULL && actRoot->rNode == NULL) {
          actRoot->rNode = nodeConstructor(strToDouble(expression), VALUE);
        } else if(operator->rNode != NULL /* && operator->rNode->operator == UNARY_MINUS */)
          operator->rNode->rNode = nodeConstructor(strToDouble(expression), VALUE);
        else
          operator->rNode = nodeConstructor(strToDouble(expression), VALUE);
      }
      afterOperator = false;
    }
    if(**expression != '\0')
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
  assert(calculate("((5.42+6.01)/( -3.05)*7.06- -0.59)") == -25.8676393442622938);
}