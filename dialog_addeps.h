#ifndef DIALOG_ADDEPS_H
#define DIALOG_ADDEPS_H

#include <QDialog>

namespace Ui {
class Dialog_addEps;
}

class Dialog_addEps : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_addEps(QWidget *parent = 0);
    ~Dialog_addEps();

    void* retPrt() const;                  // повернення вказiвника з даними
    void* retEdit(void* DataPoint) const;  // повернення вiдедагованого вказiвника
    QString retUp() const;                 // повернення ключового поля верхнього рiвня
    QString retUpUp() const;               // повернення ключового поля верхнього рiвня

public slots:
    void when_update_activity (QString info);       // текст у дiалозi
    void when_set_data_forEdit (void *DataPoint);   // передача вказiвника для заповнення полiв на редагування

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_OK_clicked();

private:
    Ui::Dialog_addEps *ui;

    QString info__;     // змiнна тексту Дiалогу
    void *DataPoint__;  // змiнна вказiвник
};

#endif // DIALOG_ADDEPS_H
