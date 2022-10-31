#include "mainwindow.h"

#include <QtGui>

#include "ui_mainwindow.h"

std::string res_string = "";
QString oper = "*+-/(^,";
QString buff = "";
QString funcions[10] = {"mod",  "sin",  "cos",  "tan", "asin",
                        "acos", "atan", "sqrt", "log", "ln"};
int check_open = 0;
int check_error = 0;
char x_temp[255] = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  MainWindow::setWindowTitle("Calc");
  QRegularExpressionValidator *r = new QRegularExpressionValidator(
      QRegularExpression("\[-]{0,1}\\d{0,}\[.]{0,1}\\d{0,}"));
  QRegularExpressionValidator *rr =
      new QRegularExpressionValidator(QRegularExpression("\[-]{0,1}\\d{0,}"));
  ui->lineEdit->setValidator(rr);
  ui->lineEdit_2->setValidator(rr);
  ui->lineEdit_x->setValidator(r);
  ui->pushButton_10->setFixedSize(393, 32);

  int x1 = ui->lineEdit->text().toInt(), x2 = ui->lineEdit_2->text().toInt();

  pictWidth = 558;
  pictHeight = 389;
  step = 0.1;
  leftX = x1 < x2 ? x1 : x2;
  rightX = x1 < x2 ? x2 : x1;

  std::vector<QPushButton *> button_number = {
      ui->pushButton_X, ui->pushButton_0, ui->pushButton_1, ui->pushButton_2,
      ui->pushButton_3, ui->pushButton_4, ui->pushButton_5, ui->pushButton_6,
      ui->pushButton_7, ui->pushButton_8, ui->pushButton_9};

  std::vector<QPushButton *> button_fun = {
      ui->pushButton_mod,  ui->pushButton_sin,  ui->pushButton_cos,
      ui->pushButton_tan,  ui->pushButton_asin, ui->pushButton_acos,
      ui->pushButton_atan, ui->pushButton_sqrt, ui->pushButton_log,
      ui->pushButton_ln};

  for (auto i = 0; i < (int)button_number.size(); i++)
    connect(button_number[i], SIGNAL(clicked()), this, SLOT(button_numbers()));

  for (auto i = 0; i < (int)button_fun.size(); i++)
    connect(button_fun[i], SIGNAL(clicked()), this, SLOT(button_func()));

  connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(button_slide()));
  connect(ui->pushButton_up, SIGNAL(clicked()), this, SLOT(button_slide()));

  connect(ui->pushButton_dot, SIGNAL(clicked()), this,
          SLOT(button_dot_click()));

  connect(ui->pushButton_open, SIGNAL(clicked()), this,
          SLOT(button_open_close()));
  connect(ui->pushButton_close, SIGNAL(clicked()), this,
          SLOT(button_open_close()));

  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(button_oper()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(button_oper()));
  connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(button_oper()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(button_oper()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(button_oper()));

  connect(ui->pushButton_unar, SIGNAL(clicked()), this,
          SLOT(revers_polaris_button()));

  connect(ui->pushButton_comma, SIGNAL(clicked()), this, SLOT(button_comma()));

  connect(ui->pushButton_delete, SIGNAL(clicked()), this,
          SLOT(delete_button()));
  connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(clear_show()));

  connect(ui->pushButton_ravno, SIGNAL(clicked()), this,
          SLOT(calculate_result()));

  connect(ui->pushButton_enter, SIGNAL(clicked()), this, SLOT(button_enter()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::recountPixels() {
  onePixelX = 558.0 / (rightX - leftX);
  onePixelY = 389.0 / (rightY - leftY);
  Ox = fabs(leftX);
  Oy = rightY;
}

void MainWindow::drawGraph(bool notEmpty) {
  QPixmap graph(558, 389);
  QPainter paint;
  paint.begin(&graph);

  paint.eraseRect(0, 0, 558, 389);
  paint.setPen(QPen(Qt::gray, 1));
  paint.drawLine(Ox * onePixelX, 0, Ox * onePixelX, pictHeight);
  paint.drawLine(0, Oy * onePixelY, pictWidth, Oy * onePixelY);

  paint.setPen(QPen(Qt::black, 3));
  if (rightX >= 10) {
    int x_step = (rightX + abs(leftX)) / 20;
    for (int i = leftX; i <= rightX; i += x_step) {
      paint.drawPoint((i + Ox) * onePixelX, Oy * onePixelY);
      if (i < 0)
        paint.drawText((i + Ox) * onePixelX - 6, Oy * onePixelY + 18,
                       QString::number(i));
      else if (i > 0)
        paint.drawText((i + Ox) * onePixelX - 2, Oy * onePixelY - 4,
                       QString::number(i));
    }
  } else {
    double x_step = (rightX + abs(leftX)) / 20;
    for (double i = leftX; i < rightX; i += x_step) {
      paint.drawPoint((i + Ox) * onePixelX, Oy * onePixelY);
      if (i < -1e-6)
        paint.drawText((i + Ox) * onePixelX - 6, Oy * onePixelY + 18,
                       credit.dtoqs(i));
      else if (i > 1e-6)
        paint.drawText((i + Ox) * onePixelX - 2, Oy * onePixelY - 4,
                       credit.dtoqs(i));
    }
  }

  if (rightY >= 10) {
    int y_step = (rightY + abs(leftY)) / 20;
    for (int i = leftY; i <= rightY; i += y_step) {
      paint.drawPoint(Ox * onePixelX, (i + Oy) * onePixelY);
      if (i < 0)
        paint.drawText(Ox * onePixelX - 30, (i + Oy) * onePixelY + 4,
                       QString::number(-i));
      else if (i > 0)
        paint.drawText(Ox * onePixelX + 8, (i + Oy) * onePixelY + 4,
                       QString::number(-i));
    }
  } else {
    double y_step = (rightY + abs(leftY)) / 20;
    for (double i = leftY; i < rightY; i += y_step) {
      paint.drawPoint(Ox * onePixelX, (i + Oy) * onePixelY);
      if (i < -1e-6)
        paint.drawText(Ox * onePixelX - 35, (i + Oy) * onePixelY + 4,
                       credit.dtoqs(-i));
      else if (i > 1e-6)
        paint.drawText(Ox * onePixelX + 8, (i + Oy) * onePixelY + 4,
                       credit.dtoqs(-i));
    }
  }

  if (!notEmpty) {
    paint.end();
    ui->label->setPixmap(graph);
    return;
  }

  paint.setPen(QPen(Qt::red, 2, Qt::SolidLine));
  paint.setRenderHint(QPainter::Antialiasing, true);
  QPainterPath path;
  bool first = 1;

  for (double i = leftX + step; i <= rightX; i += step) {
    memset(x_temp, '\0', 255);
    std::string tem = QString::number(i, 'g', 3).toStdString();
    strlcpy(x_temp, tem.c_str(), tem.size() + 1);
    char res_c[512];
    strlcpy(res_c, res_string.c_str(), res_string.size() + 1);
    double res_n = calc(res_c, x_temp);
    if (!isnan(res_n) && !isinf(res_n)) {
      if (first) {
        path.moveTo((i + Ox) * onePixelX, (Oy - res_n) * onePixelY);
        first = 0;
      } else {
        path.lineTo((i + Ox) * onePixelX, (Oy - res_n) * onePixelY);
      }
    }
  }

  paint.setPen(QPen(Qt::darkMagenta, 2, Qt::SolidLine));
  paint.drawPath(path);

  paint.end();
  ui->label->setPixmap(graph);
}

int MainWindow::check_numb(QString sear) {
  return sear.size()
             ? (sear[sear.size() - 1] >= '0' && sear[sear.size() - 1] <= '9') ||
                   sear[sear.size() - 1] == 'X'
             : 0;
}

void MainWindow::button_enter() {
  QString old_label = ui->pre_result_show->toPlainText();
  QString label = ui->result_show->toPlainText();
  if (old_label.size()) {
    if (check_numb(label)) {
      label += '*';
      res_string += " * ";
    }
    ui->result_show->setText(label + old_label);
    ui->pre_result_show->clear();
    res_string += old_label.toStdString();
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }

  std::vector<QPushButton *> button_number = {
      ui->pushButton_0, ui->pushButton_1, ui->pushButton_2, ui->pushButton_3,
      ui->pushButton_4, ui->pushButton_5, ui->pushButton_6, ui->pushButton_7,
      ui->pushButton_8, ui->pushButton_9};

  std::vector<QPushButton *> button_fun = {
      ui->pushButton_mod,  ui->pushButton_sin,  ui->pushButton_cos,
      ui->pushButton_tan,  ui->pushButton_asin, ui->pushButton_acos,
      ui->pushButton_atan, ui->pushButton_sqrt, ui->pushButton_log,
      ui->pushButton_ln};

  std::vector<QPushButton *> button_oper = {
      ui->pushButton_plus, ui->pushButton_minus, ui->pushButton_mul,
      ui->pushButton_div, ui->pushButton_pow};

  int numb = event->key();
  if (numb >= 48 && numb <= 57) {
    button_number[numb - 48]->animateClick();
  } else if (numb == 'X') {
    ui->pushButton_X->animateClick();
  } else if (numb == Qt::Key_Backspace) {
    ui->pushButton_delete->animateClick();
  } else if (numb == '(') {
    ui->pushButton_open->animateClick();
  } else if (numb == ')') {
    ui->pushButton_close->animateClick();
  } else if (numb == '+' || numb == '-' || numb == '*' || numb == '/' ||
             numb == '^') {
    for (int i = 0; i < 5; i++) {
      if (QChar(numb) == button_oper[i]->text()) {
        button_oper[i]->animateClick();
      }
    }
  } else if (numb == '.') {
    ui->pushButton_dot->animateClick();
  } else if (numb == ',') {
    ui->pushButton_comma->animateClick();
  } else if (numb == Qt::Key_Return) {
    if (ui->pre_result_show->toPlainText().size())
      ui->pushButton_enter->animateClick();
    else
      ui->pushButton_ravno->animateClick();
  } else if (numb == Qt::Key_Escape) {
    ui->pushButton_AC->animateClick();
  } else if (event->text() == "±") {
    ui->pushButton_unar->animateClick();
  } else if (numb >= 'A' && numb <= 'Z') {
    buff += QChar(numb);
    if (buff.size() >= 2) {
      int g = 12;
      for (int i = 0; i < 10 && g == 12; i++) {
        if (funcions[i].contains(buff.toLower())) {
          g = i;
        }
      }
      if (g != 12) {
        button_fun[g]->animateClick();
      }
      buff = "";
    }
  }
}

void MainWindow::revers_polaris_button() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QString str = ui->pre_result_show->toPlainText();
  if (!str.size()) {
    ui->pre_result_show->setText("-");
  } else {
    if (str[0] != '-')
      ui->pre_result_show->setText('-' + str);
    else if (str[0] == '-')
      ui->pre_result_show->setText(str.remove("-"));
  }
}

void MainWindow::delete_button() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QString label = ui->result_show->toPlainText();
  QString old_label = ui->pre_result_show->toPlainText();
  if (old_label.size()) {
    ui->pre_result_show->setText(old_label.left(old_label.length() - 1));
  } else if (!old_label.size() && label.size()) {
    if (label[label.size() - 1] == '(' && check_open) {
      check_open -= 1;
    }
    ui->check_ope_label->setText(QString::number(check_open));
    ui->result_show->setText(label.left(label.length() - 1));
    if (res_string[res_string.size() - 1] == ' ') {
      res_string.erase(res_string.size() - 3);
    } else {
      res_string.resize(res_string.size() - 1);
    }
  }
}

void MainWindow::clear_show() {
  ui->result_show->clear();
  ui->pre_result_show->clear();
  res_string.clear();
  leftY = -10;
  rightY = 10;
  recountPixels();
  drawGraph();
  check_open = 0;
}

void MainWindow::button_open_close() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QPushButton *button = (QPushButton *)sender();
  QString skob_text = button->text();
  QString old_label = ui->pre_result_show->toPlainText();
  ui->pre_result_show->clear();
  ui->result_show->setText(ui->result_show->toPlainText() + old_label);
  res_string += old_label.toStdString();
  QString s = ui->result_show->toPlainText();
  if (skob_text == '(') {
    if (!s.size()) {
      ui->result_show->setText(s + '(');
      res_string += " ( ";
      check_open += 1;
      ui->check_ope_label->setText(QString::number(check_open));
    } else if (check_numb(s) || s[s.size() - 1] == ')') {
      ui->result_show->setText(s + "*(");
      res_string += " *  ( ";
      check_open += 1;
      ui->check_ope_label->setText(QString::number(check_open));
    } else {
      ui->result_show->setText(s + '(');
      res_string += " ( ";
      check_open += 1;
      ui->check_ope_label->setText(QString::number(check_open));
    }
    ui->pre_result_show->clear();
  } else if (skob_text == ')' && check_open) {
    if (s.size() && !oper.contains(s[s.size() - 1])) {
      ui->result_show->setText(s + ')');
      res_string += " ) ";
      check_open -= 1;
      ui->check_ope_label->setText(QString::number(check_open));
    }
  }
}

void MainWindow::button_func() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QPushButton *button = (QPushButton *)sender();
  QString func_text = button->text();
  QString old_label = ui->pre_result_show->toPlainText();
  QString s = ui->result_show->toPlainText();
  ui->pre_result_show->clear();
  ui->result_show->setText(s + old_label);
  res_string += old_label.toStdString();
  s = ui->result_show->toPlainText();
  if (!s.size() && !old_label.size()) {
    ui->result_show->setText(func_text + '(');
    res_string += func_text.toStdString() + " ( ";
    check_open += 1;
    ui->check_ope_label->setText(QString::number(check_open));
  } else {
    if (check_numb(s) || s[s.size() - 1] == ')') {
      ui->result_show->setText(s + '*' + func_text + '(');
      res_string += " * " + func_text.toStdString() + " ( ";
      check_open += 1;
      ui->check_ope_label->setText(QString::number(check_open));
    } else {
      ui->result_show->setText(s + func_text + '(');
      res_string += ' ' + func_text.toStdString() + " ( ";
      check_open += 1;
      ui->check_ope_label->setText(QString::number(check_open));
    }
  }
}

void MainWindow::button_oper() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QPushButton *button = (QPushButton *)sender();
  QString oper_text = button->text();
  QString old_label = ui->pre_result_show->toPlainText();
  QString label = ui->result_show->toPlainText();
  if (!label.size()) {
    ui->pre_result_show->clear();
    if (!old_label.size() && !label.size()) {
      old_label = "0";
    }
    ui->result_show->setText(ui->result_show->toPlainText() + old_label +
                             oper_text);
    res_string += old_label.toStdString() + " " + oper_text.toStdString() + " ";
  } else if (!(oper.contains(label[label.size() - 1])) || old_label.size()) {
    ui->pre_result_show->clear();
    ui->result_show->setText(ui->result_show->toPlainText() + old_label +
                             oper_text);
    res_string += old_label.toStdString() + " " + oper_text.toStdString() + " ";
  }
}

void MainWindow::button_comma() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QString old_label = ui->pre_result_show->toPlainText();
  QString label = ui->result_show->toPlainText();
  if ((check_numb(label) || check_numb(old_label)) && check_open) {
    ui->result_show->setText(label + old_label);
    res_string += old_label.toStdString();
    label = ui->result_show->toPlainText();
    ui->pre_result_show->clear();
    ui->result_show->setText(label + ',');
    res_string += " , ";
  }
}

void MainWindow::calculate_result() {
  QString label = ui->result_show->toPlainText();
  QString old_label = ui->pre_result_show->toPlainText();
  res_string += old_label.toStdString();
  if (label.size() && !check_open) {
    memset(x_temp, '\0', 255);
    if (label.contains('X')) {
      strlcpy(x_temp, ui->lineEdit_x->text().toStdString().c_str(),
              ui->lineEdit_x->text().length() + 1);
    }
    char res_c[512];
    strlcpy(res_c, res_string.c_str(), res_string.size() + 1);
    double res_n = calc(res_c, x_temp);
    QString res;
    if (isnan(res_n) || isinf(res_n)) {
      res = "Ошибка вычисления!";
      check_error = 1;
    } else {
      res = QString::number(res_n, 'g', 10);
    }
    ui->result_show->clear();
    res_string.clear();
    ui->pre_result_show->setText(res);
    check_open = 0;
  }
}

void MainWindow::button_slide() {
  QPushButton *button = (QPushButton *)sender();
  if (button->text() == '>') {
    setFixedSize(574, 614);
    button->setText("<");
    ui->pushButton_10->setFixedSize(577, 32);
  } else if (button->text() == '<') {
    setFixedSize(389, 614);
    button->setText(">");
    ui->pushButton_10->setFixedSize(393, 32);
  } else if (button->text() == '^') {
    setFixedSize(389, 614);
    ui->pushButton->setText(">");
    ui->pushButton_10->setFixedSize(393, 32);
  }
}

void MainWindow::button_dot_click() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  if (!ui->pre_result_show->toPlainText().size())
    ui->pre_result_show->setText("0.");
  else if (!(ui->pre_result_show->toPlainText().contains('.')))
    ui->pre_result_show->setText(ui->pre_result_show->toPlainText() + '.');
}

void MainWindow::button_numbers() {
  if (check_error) {
    ui->pre_result_show->clear();
    ui->result_show->clear();
    check_error = 0;
  }
  QPushButton *button = (QPushButton *)sender();
  QString number_text = button->text();
  QString label = ui->result_show->toPlainText();
  QString old_label = ui->pre_result_show->toPlainText();
  if (label.size() > 0 &&
      (label[label.size() - 1] == ')' || label[label.size() - 1] == 'X')) {
    ui->result_show->setText(label + '*');
    res_string += " * ";
  }
  label = ui->result_show->toPlainText();
  if (number_text == 'X') {
    if (label.size())
      if (check_numb(label) || label[label.size() - 1] == 'X') {
        ui->result_show->setText(label + '*');
        res_string += " * ";
      }
    label = ui->result_show->toPlainText();
    if (old_label.size()) {
      ui->result_show->setText(label + old_label + "*X");
      res_string += ' ' + old_label.toStdString() + " * X";
      ui->pre_result_show->clear();
    } else if (!old_label.size()) {
      ui->result_show->setText(label + 'X');
      res_string += 'X';
    }
  } else {
    ui->pre_result_show->setText(ui->pre_result_show->toPlainText() +
                                 number_text);
  }
}

void MainWindow::on_pushButton_credit_show_clicked() { credit.show(); }

void MainWindow::on_pushButton_deposit_show_clicked() { deposit.show(); }

void MainWindow::on_pushButton_10_clicked() {
  QString label = ui->result_show->toPlainText();
  QString old_label = ui->pre_result_show->toPlainText();
  int x1 = ui->lineEdit->text().toInt(), x2 = ui->lineEdit_2->text().toInt();
  leftX = x1 < x2 ? x1 : x2;
  rightX = x1 < x2 ? x2 : x1;
  leftX = leftX == 0 ? -1 : leftX;
  rightX = rightX == 0 ? 1 : rightX;

  if (label.contains('X')) {
    ui->result_show->setText(label + old_label);
    ui->pre_result_show->clear();
    leftY = -3;
    rightY = 3;

    for (double i = leftX + step; i <= rightX; i += step) {
      memset(x_temp, '\0', 255);
      std::string tem = QString::number(i, 'g', 3).toStdString();
      strlcpy(x_temp, tem.c_str(), tem.size() + 1);
      char res_c[512];
      strlcpy(res_c, res_string.c_str(), res_string.size() + 1);
      double res_n = calc(res_c, x_temp);
      if (!isnan(res_n) && !isinf(res_n)) {
        if (res_n > rightY) {
          rightY = res_n + 2;
        }
        if (res_n < leftY) {
          leftY = res_n - 2;
        }
      }
    }
    recountPixels();
    ui->pushButton_10->setFixedSize(577, 32);
    ui->pushButton->setText("<");
    setFixedSize(573, 1045);
    drawGraph(1);
  }
}
