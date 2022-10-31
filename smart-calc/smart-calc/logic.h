#ifndef SRC_SMART_CALC_LOGIC_H
#define SRC_SMART_CALC_LOGIC_H

#ifdef __splusplus
extern "C" {
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR res = 1;
#define OK int res = 0;

typedef struct Node {
  char value[512];
  struct Node *next;
} Node;

typedef struct Rele {
  double value;
  struct Rele *next;
} Rele;

int push_re(Rele **head, double value);
double pop_re(Rele **head);
int push(Node **head, char value[512]);
void pop(Node **head, char value[512]);
void drop(Node **head);
void drop_re(Rele **head);
int peek(const Node *head);
int peek_re(const Rele *head);
int search_func(char c, int a);
int search_oper(char c);
int prior_oper(char oper);
int equal_oper(char oper_1, char oper_2);
void steck_to_out(Node **Steck, Node **Out);
void sort_oper(Node **Out, Node **Steck, char oper[512]);
int if_func(char fun[4]);
void print_steck(Node *head);
void relocate(Node **src, Node **dst);
int parser(char string[512], Node **out, char x_temp[255]);
double calc(char strok[512], char x_temp[255]);
double calc_a_mp(double proc, double sum, double month);
double calc_a_sum(double mon_price, double month);
void calc_d(double proc, double sum, double month, double *max_mp,
            double *min_mp, double *calc_sum, double *calc_proc);

#ifdef __splusplus
}
#endif
#endif  // SRC_SMART_CALC_LOGIC_H
