#include "logic.h"

int push_re(Rele **head, double value) {
  OK Rele *temp = malloc(sizeof(Rele));
  if (temp == NULL) {
    ERROR
  } else {
    temp->next = *head;
    temp->value = value;
    *head = temp;
  }
  return res;
}

double pop_re(Rele **head) {
  Rele *temp = *head;
  *head = (*head)->next;
  double value = temp->value;
  free(temp);
  return value;
}

int push(Node **head, char value[512]) {
  OK Node *temp = malloc(sizeof(Node));
  if (temp == NULL) {
    ERROR
  } else {
    temp->next = *head;
    strcpy(temp->value, value);
    *head = temp;
  }
  return res;
}

void pop(Node **head, char value[512]) {
  Node *temp = *head;
  *head = (*head)->next;
  strcpy(value, temp->value);
  free(temp);
}

void drop(Node **head) {
  Node *temp = *head;
  *head = (*head)->next;
  free(temp);
}

void drop_re(Rele **head) {
  Rele *temp = *head;
  *head = (*head)->next;
  free(temp);
}

int peek(const Node *head) { return head == NULL; }

int peek_re(const Rele *head) { return head == NULL; }

int search_oper(char c) {
  OK char sear[] = "+-*/^";
  for (int i = 0; !res && sear[i]; res = c == sear[i++]) {
  }
  return res;
}

int prior_oper(char oper) {
  return oper == '+' || oper == '-'   ? 1
         : oper == '*' || oper == '/' ? 2
         : oper == '^'                ? 3
                                      : 0;
}

int equal_oper(char oper_1, char oper_2) {
  return prior_oper(oper_1) >= prior_oper(oper_2);
}

void steck_to_out(Node **Steck, Node **Out) {
  char value[512] = "";
  pop(Steck, value);
  push(Out, value);
}

void sort_oper(Node **Out, Node **Steck, char oper[512]) {
  OK while (!res && !peek(*Steck)) {
    if (oper[0] == '^' && *(*Steck)->value == '^') {
      push(Steck, oper);
      ERROR
    } else if (equal_oper(*(*Steck)->value, oper[0])) {
      steck_to_out(Steck, Out);
    } else {
      push(Steck, oper);
      ERROR
    }
  }
  if (peek(*Steck) && !res) push(Steck, oper);
}

int if_func(char fun[5]) {
  OK char func[10][5] = {"sin",  "cos",  "tan", "acos", "asin",
                         "atan", "sqrt", "ln",  "log",  "mod"};
  for (int i = 0; i < 10 && !res; i++) {
    res = !strcmp(fun, func[i]);
  }
  return res;
}

void relocate(Node **src, Node **dst) {
  for (; !peek(*src); steck_to_out(src, dst)) {
  }
}

double calc(char strok[512], char x_temp[255]) {
  Node *src = NULL;
  double res = parser(strok, &src, x_temp);
  Node *realese = NULL;
  Rele *itog = NULL;
  relocate(&src, &realese);
  while (!peek(realese) && !res) {
    char temp[512] = "";
    pop(&realese, temp);
    if (!strcmp(temp, "+")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value += t;
    } else if (!strcmp(temp, "-")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value -= t;
    } else if (!strcmp(temp, "/")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value /= t;
    } else if (!strcmp(temp, "*")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value *= t;
    } else if (!strcmp(temp, "^")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value = pow(itog->value, t);
    } else if (!strcmp(temp, "sin")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = sin(itog->value);
    } else if (!strcmp(temp, "cos")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = cos(itog->value);
    } else if (!strcmp(temp, "tan")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = tan(itog->value);
    } else if (!strcmp(temp, "acos")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = acos(itog->value);
    } else if (!strcmp(temp, "asin")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = asin(itog->value);
    } else if (!strcmp(temp, "atan")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = atan(itog->value);
    } else if (!strcmp(temp, "sqrt")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = sqrt(itog->value);
    } else if (!strcmp(temp, "ln")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = log10(itog->value);
    } else if (!strcmp(temp, "mod")) {
      double t = pop_re(&itog);
      if (peek_re(itog))
        ERROR
      else
        itog->value = fmod(itog->value, t);
    } else if (!strcmp(temp, "log")) {
      if (peek_re(itog))
        ERROR
      else
        itog->value = log(itog->value);
    } else {
      push_re(&itog, atof(temp));
    }
  }
  double result = 0.F;
  if (!peek(realese)) {
    ERROR
    for (; !peek(realese); drop(&realese)) {
    }
  }
  if (!res) result = pop_re(&itog);
  if (!peek_re(itog)) {
    ERROR
    for (; !peek_re(itog); drop_re(&itog)) {
    }
  }
  return !res ? result : NAN;
}

int parser(char string[512], Node **out, char x_temp[255]) {
  OK Node *steck = NULL;
  char *temp = strtok(string, " ");
  while (temp != NULL && !res) {
    if ((*temp == '-' || *temp == '+') && strlen(temp) > 1) {
      push(out, temp);
    } else if (*temp > 47 && *temp < 59) {
      push(out, temp);
    } else if (search_oper(*temp)) {
      char t[2] = "";
      t[0] = *temp;
      if (peek(steck))
        push(&steck, t);
      else
        sort_oper(out, &steck, t);
    } else if (temp[0] == 'X') {
      push(out, x_temp);
    } else if (if_func(temp)) {
      push(&steck, temp);
    } else if (*temp == '(') {
      push(&steck, temp);
    } else if (*temp == ')') {
      while (!peek(steck) && *steck->value != '(') steck_to_out(&steck, out);
      if (peek(steck)) {
        ERROR
      } else if (*steck->value == '(') {
        drop(&steck);
        if (!peek(steck)) {
          if (if_func(steck->value)) steck_to_out(&steck, out);
        }
      }
    } else if (*temp == ',') {
    } else {
      ERROR
    }
    if (!res) temp = strtok(NULL, " ");
  }
  while (!peek(steck) && !res) {
    if (*steck->value == '(')
      ERROR
    else
      steck_to_out(&steck, out);
  }
  if (!peek(steck) || peek(*out)) ERROR
  for (; !peek(steck); drop(&steck)) {
  }
  for (; !peek(*out) && res; drop(out)) {
  }
  return res;
}

double calc_a_mp(double proc, double sum, double month) {
  proc = proc / 100.0 / 12.0;
  return sum * (proc * pow((1 + proc), month) / (pow(1 + proc, month) - 1));
}

double calc_a_sum(double mon_price, double month) { return mon_price * month; }

void calc_d(double proc, double sum, double month, double *max_mp,
            double *min_mp, double *calc_sum, double *calc_proc) {
  proc = proc / 100 / 12;
  double sum_t = sum;
  double pre_mp = sum / month;
  *max_mp = pre_mp + (sum_t * proc);
  *calc_proc = 0;
  while (sum_t > 0) {
    *calc_proc += (sum_t * proc);
    sum_t -= pre_mp;
    if (sum_t == pre_mp || sum_t < pre_mp) *min_mp = pre_mp + (sum_t * proc);
  }
  *calc_sum = *calc_proc + sum;
}
