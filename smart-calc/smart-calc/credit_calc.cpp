#include "credit_calc.h"

#include "ui_credit_calc.h"

Credit_calc::Credit_calc(QWidget *parent)
    : QWidget(parent), ui(new Ui::Credit_calc) {
  ui->setupUi(this);
  Credit_calc::setFixedSize(403, 332);
  Credit_calc::setWindowTitle("Credit Calc");
  ui->textEdit_sum->setValidator(double_reg);
  ui->textEdit_term->setValidator(double_reg);
  ui->textEdit_procent->setValidator(double_reg);
}

Credit_calc::~Credit_calc() { delete ui; }

QString Credit_calc::dtoqs(double numb) {
  std::string st = std::to_string(round(numb * 100) / 100);
  st.resize(st.size() - 4);
  return QString::fromStdString(st);
}

void Credit_calc::on_pushButton_clicked() {
  double sum = 0;
  int month = 0;
  double proc = 0;

  QString temp = ui->textEdit_sum->text();
  if (temp.size()) {
    sum = temp.toDouble();
  }
  temp = ui->textEdit_term->text();
  if (temp.size()) {
    month = temp.toInt();
  }
  if (ui->comboBox->currentText() == "Лет") {
    month *= 12;
  }
  temp = ui->textEdit_sum->text();
  if (temp.size()) {
    proc = temp.toDouble();
  }

  double calc_month_max = 0, calc_month_min = 0, calc = 0, proc_calc = 0;

  if (ui->checkBox_anu->isChecked() && sum && month && proc) {
    calc_month_max = calc_a_mp(proc, sum, month);
    calc = calc_a_sum(calc_month_max, month);
    proc_calc = calc - sum;
    ui->label_calc_month->setText(dtoqs(calc_month_max));
  } else if (sum && month && proc) {
    calc_d(proc, sum, month, &calc_month_max, &calc_month_min, &calc,
           &proc_calc);
    ui->label_calc_month->setText(dtoqs(calc_month_max) + "..." +
                                  dtoqs(calc_month_min));
  } else {
    ui->label_calc_month->setText("0.00");
  }

  ui->label_calc_proc->setText(dtoqs(proc_calc));
  ui->label_calc_sum->setText(dtoqs(calc));
}
