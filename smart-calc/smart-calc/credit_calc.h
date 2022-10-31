#ifndef CREDIT_CALC_H
#define CREDIT_CALC_H

#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QWidget>

extern "C" {
#include "logic.h"
}

namespace Ui {
class Credit_calc;
}

class Credit_calc : public QWidget {
  Q_OBJECT

 public:
  explicit Credit_calc(QWidget *parent = nullptr);
  ~Credit_calc();
  QString dtoqs(double numb);
  QRegularExpressionValidator *double_reg = new QRegularExpressionValidator(
      QRegularExpression("\\d{0,}\[.]{0,1}\\d{0,}"));

 private slots:
  void on_pushButton_clicked();

 private:
  Ui::Credit_calc *ui;
};

#endif  // CREDIT_CALC_H
