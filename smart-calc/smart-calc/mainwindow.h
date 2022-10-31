#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "credit_calc.h"
#include "deposit_calc.h"

extern "C" {
#include "logic.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  int check_numb(QString sear);
  void drawGraph(bool notEmpty = 0);
  void recountPixels();

 private:
  Ui::MainWindow *ui;
  Credit_calc credit;
  Deposit_calc deposit;
  double leftX, rightX;
  double leftY, rightY;
  int pictWidth, pictHeight;
  double step;
  double onePixelX, onePixelY;
  double Ox, Oy;

 public slots:
  void button_numbers();
  void calculate_result();
  void button_func();
  void button_oper();
  void delete_button();
  void button_open_close();
  void clear_show();
  void button_dot_click();
  void revers_polaris_button();
  void button_comma();
  void button_slide();
  void button_enter();

 protected:
  virtual void keyPressEvent(QKeyEvent *event);

 private slots:
  void on_pushButton_credit_show_clicked();
  void on_pushButton_deposit_show_clicked();
  void on_pushButton_10_clicked();
};
#endif  // MAINWINDOW_H
