#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void DelEdit_label (Ui::MainWindow *ui, QString newName, QString oldName);  // змiна тексту на кнопках-континентах

public slots:
    void when_set_label (QString nameCont, QString btnText);                    // передача тексту для змiни на кнопцi-континентi


private:
    Ui::MainWindow *ui;

    QString nameCont__; // змiнна iм'я континенту
    QString btnText__;  // змiнна iм'я кнопки

private slots:
    void ElemAdd();     // додавання елемента
    void ElemDel();     // видалення елемента
    void ElemEdit();    // редагування елемента

    void on_actionAdd_ctr_triggered();
    void on_actionAdd_eps_triggered();

    void on_pushButton_find_clicked();
    void on_pushButton_findHost_clicked();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();

signals:
    void update_activity (QString);         // сигнал для передачi тексту для Дiалога
    void set_rules ();                      // сигнал встановлення iнформацiї по введенню
    void set_level(int);                    // сигнал передачi рiвня елемента
    void set_data_forEdit (void *);         // сигнал передачi вказiвника для заповнення полiв на редагування
    void set_label_cont (QString, QString); // змiна тексту на кнопцi-континентi
};

#endif // MAINWINDOW_H
