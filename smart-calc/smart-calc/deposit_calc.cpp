#include "deposit_calc.h"

#include "ui_deposit_calc.h"

Deposit_calc::Deposit_calc(QWidget *parent)
    : QWidget(parent), ui(new Ui::Deposit_calc) {
  ui->setupUi(this);
  Deposit_calc::setFixedSize(431, 556);
  Deposit_calc::setWindowTitle("Deposit calc");

  QDate tr = QDate::currentDate();
  ui->textEdit_sum->setValidator(credit.double_reg);
  ui->textEdit_proc->setValidator(credit.double_reg);
  ui->textEdit_nalog->setValidator(credit.double_reg);
  ui->textEdit_period->setValidator(credit.double_reg);
  ui->textEdit_is->setValidator(credit.double_reg);
  ui->textEdit_with->setValidator(credit.double_reg);
  ui->dateEdit->setDate(tr);
  ui->dateEdit_is->setDate(tr);
  ui->dateEdit_with->setDate(tr);
}

Deposit_calc::~Deposit_calc() { delete ui; }

void Deposit_calc::on_pushButton_is_add_clicked() {
  is_add++;
  QString st = ui->comboBox_is->currentText() + " " +
               ui->dateEdit_is->date().toString("dd/M/yyyy") + " " +
               ui->textEdit_is->text() + "\n";
  ui->listWidget_is->addItem(st);
}

void Deposit_calc::on_pushButton_is_delete_clicked() {
  if (is_add > -1) {
    int a = ui->listWidget_is->currentRow();
    ui->listWidget_is->takeItem(a >= 0 ? a : is_add);
    is_add--;
  }
}

void Deposit_calc::on_pushButton_with_add_clicked() {
  with_add++;
  QString st = ui->comboBox_with->currentText() + " " +
               ui->dateEdit_with->date().toString("dd/M/yyyy") + " " +
               ui->textEdit_with->text() + "\n";
  ui->listWidget_with->addItem(st);
}

void Deposit_calc::on_pushButton_with_delete_clicked() {
  if (with_add > -1) {
    int a = ui->listWidget_with->currentRow();
    ui->listWidget_with->takeItem(a >= 0 ? a : with_add);
    with_add--;
  }
}

void Deposit_calc::parser_list(QListWidgetItem *item, int *check, QDate *date,
                               double *sum) {
  QString st = item->text(), check_t = "";
  int in = 0;
  for (int i = 0; !in; in = (st[i] >= '0' && st[i] <= '9') ? i : 0, i++) {
    check_t += st[i];
  }
  check_t.resize(check_t.size() - 2);
  QString list_t[7] = {"Раз в день",        "Раз в неделю",   "Раз в месяц",
                       "Раз в квартал",     "Раз в пол года", "Раз в год",
                       "Разовое пополнение"};
  for (int i = 0; i < 7 && !*check;
       *check = list_t[i] == check_t ? i : 0, i++) {
  }
  check_t = "";
  for (; st[in] != ' '; check_t += st[in], in++) {
  }
  int z = check_t.size() == 10;
  date->setDate(check_t.mid(5 + z).toInt(), check_t.mid(3, 1 + z).toInt(),
                check_t.mid(0, 2).toInt());
  *sum = st.mid(in + 1).toDouble();
}

void Deposit_calc::on_pushButton_calc_clicked() {
  QDate te = ui->dateEdit->date();
  QString sum_t = ui->textEdit_sum->text();
  QString period_t = ui->textEdit_period->text();
  QString proc_t = ui->textEdit_proc->text();
  QString nalog_t = ui->textEdit_nalog->text();

  double sum = 0, proc = 0, nalog = 0;
  int period_s = 0, period = 0;
  year = te.daysInYear();

  sum = sum_t.toDouble();
  period = period_t.toInt();
  proc = proc_t.toDouble() * 0.01;
  nalog = nalog_t.toDouble() * 0.01;

  if (ui->comboBox_period->currentIndex() == 1) {
    int day_t = te.day();
    period_s += te.daysInMonth() - day_t;
    te = te.addMonths(1);
    for (int i = period - 1; i > 0; i--) {
      period_s += te.daysInMonth();
      te = te.addMonths(1);
    }
    period_s += day_t;
  } else if (ui->comboBox_period->currentIndex() == 2) {
    int day_t = te.dayOfYear();
    period_s += year - day_t;
    te = te.addYears(1);
    for (int i = period - 1; i > 0; i--) {
      period_s += te.daysInYear();
      te = te.addYears(1);
    }
    period_s += day_t;
  } else {
    period_s = period;
  }

  int check = ui->comboBox_period_v->currentIndex();
  te = ui->dateEdit->date();

  int ch[is_add + 1];
  QDate dt[is_add + 1];
  double su[is_add + 1];
  int tem[is_add + 1];
  for (int i = 0; i <= is_add;
       ch[i] = 0, dt[i] = QDate::currentDate(), su[i] = 0, tem[i++] = -1) {
  }
  for (int i = 0; i <= is_add && is_add > -1; i++) {
    parser_list(ui->listWidget_is->item(i), &ch[i], &dt[i], &su[i]);
  }

  int cw[with_add + 1];
  QDate dw[with_add + 1];
  double sw[with_add + 1];
  int tew[with_add + 1];
  for (int i = 0; i <= with_add;
       cw[i] = 0, dw[i] = QDate::currentDate(), sw[i] = 0, tew[i++] = -1) {
  }
  for (int i = 0; i <= with_add && with_add > -1; i++) {
    parser_list(ui->listWidget_with->item(i), &cw[i], &dw[i], &sw[i]);
  }

  double calc_proc = 0, calc_nalog = 0;

  for (int i = 0, j = 0; i <= period_s + 1 && (sum && period);
       i++, j++, te = te.addDays(1)) {
    year = te.daysInYear();
    month = te.daysInMonth();
    int reg[7] = {1, week, month, year / 4, year / 2, year, period_s};

    for (int j = 0; j <= is_add && is_add > -1; j++) {
      if (ch[j] < 6 && te >= dt[j]) {
        tem[j] += 1;
      }
      if (ch[j] == 6) {
        if (te == dt[j]) {
          sum += su[j];
        }
      } else if (tem[j] > -1 && tem[j] % reg[ch[j]] == 0) {
        sum += su[j];
      }
    }

    for (int j = 0; j <= with_add && with_add > -1; j++) {
      if (cw[j] < 6 && te >= dw[j]) {
        tew[j] += 1;
      }
      if (cw[j] == 6) {
        if (te == dw[j]) {
          sum -= sw[j];
        }
      } else if (tew[j] > -1 && tew[j] % reg[cw[j]] == 0) {
        if (sum >= sw[j]) {
          sum -= sw[j];
        }
      }
    }

    if (j >= reg[check]) {
      double temp_proc = sum * proc * reg[check] / year;
      double temp_nalog = temp_proc * nalog;
      calc_nalog += temp_nalog;
      calc_proc += temp_proc;
      if (ui->checkBox_capital->isChecked()) sum += temp_proc - temp_nalog;
      j = 0;
    }
  }

  ui->textEdit_calc_proc->setText(credit.dtoqs(calc_proc));
  ui->textEdit_calc_nalog->setText(credit.dtoqs(calc_nalog));
  ui->textEdit_calc_sum->setText(credit.dtoqs(sum));
}

void Deposit_calc::on_pushButton_list_add_clicked() {
  ui->pushButton_is_add->setVisible(1);
  ui->pushButton_is_delete->setVisible(1);
  ui->comboBox_is->setVisible(1);
  ui->dateEdit_is->setVisible(1);
  ui->label_is->setVisible(1);
  ui->textEdit_is->setVisible(1);
  ui->listWidget_is->setVisible(1);
  ui->pushButton_with_add->setVisible(0);
  ui->pushButton_with_delete->setVisible(0);
  ui->comboBox_with->setVisible(0);
  ui->dateEdit_with->setVisible(0);
  ui->label_with->setVisible(0);
  ui->textEdit_with->setVisible(0);
  ui->listWidget_with->setVisible(0);
  Deposit_calc::setFixedSize(1007, 556);
}

void Deposit_calc::on_pushButton_list_with_clicked() {
  ui->pushButton_is_add->setVisible(0);
  ui->pushButton_is_delete->setVisible(0);
  ui->comboBox_is->setVisible(0);
  ui->dateEdit_is->setVisible(0);
  ui->label_is->setVisible(0);
  ui->textEdit_is->setVisible(0);
  ui->listWidget_is->setVisible(0);
  ui->pushButton_with_add->setVisible(1);
  ui->pushButton_with_delete->setVisible(1);
  ui->comboBox_with->setVisible(1);
  ui->dateEdit_with->setVisible(1);
  ui->label_with->setVisible(1);
  ui->textEdit_with->setVisible(1);
  ui->listWidget_with->setVisible(1);
  Deposit_calc::setFixedSize(1007, 556);
}

void Deposit_calc::on_pushButton_select_clicked() {
  ui->pushButton_is_add->setVisible(0);
  ui->pushButton_is_delete->setVisible(0);
  ui->comboBox_is->setVisible(0);
  ui->dateEdit_is->setVisible(0);
  ui->label_is->setVisible(0);
  ui->textEdit_is->setVisible(0);
  ui->listWidget_is->setVisible(0);
  ui->pushButton_with_add->setVisible(0);
  ui->pushButton_with_delete->setVisible(0);
  ui->comboBox_with->setVisible(0);
  ui->dateEdit_with->setVisible(0);
  ui->label_with->setVisible(0);
  ui->textEdit_with->setVisible(0);
  ui->listWidget_with->setVisible(0);
  Deposit_calc::setFixedSize(431, 556);
}
