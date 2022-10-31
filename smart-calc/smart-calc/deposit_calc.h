#ifndef DEPOSIT_CALC_H
#define DEPOSIT_CALC_H

#include <QListWidgetItem>
#include <QWidget>

#include "credit_calc.h"

namespace Ui {
class Deposit_calc;
}

typedef struct Is_Add {
  int check;
  int date[3];
  int sum;
  struct Is_Add *next;
} Is_Add;

class Deposit_calc : public QWidget {
  Q_OBJECT

 public:
  explicit Deposit_calc(QWidget *parent = nullptr);
  ~Deposit_calc();
  Credit_calc credit;
  void parser_list(QListWidgetItem *item, int *check, QDate *date, double *sum);
  int week = 7.0;
  int month = 30;
  int year = 365.0;
  int is_add = -1;
  int with_add = -1;

 private slots:
  void on_pushButton_calc_clicked();
  void on_pushButton_list_add_clicked();
  void on_pushButton_list_with_clicked();
  void on_pushButton_select_clicked();
  void on_pushButton_is_add_clicked();
  void on_pushButton_is_delete_clicked();
  void on_pushButton_with_add_clicked();
  void on_pushButton_with_delete_clicked();

 private:
  Ui::Deposit_calc *ui;
};

#endif  // DEPOSIT_CALC_H
