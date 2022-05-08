#ifndef DIALOG_ADDCTR_H
#define DIALOG_ADDCTR_H

#include <QDialog>
#include <QString>

namespace Ui {
class Dialog_addCtr;
}

class Dialog_addCtr : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_addCtr(QWidget *parent = 0);
    ~Dialog_addCtr();

    void* retPrt() const;           // повернення вказiвника з даними
    QString retUp() const;          // повернення ключового поля верхнього рiвня
    void* retEdit(void*) const;     // повернення вiдедагованого вказiвника

public slots:
    void when_update_activity(QString info);         // текст у дiалозi
    void when_set_data_forEdit (void *DataPoint);    // передача вказiвника для заповнення полiв на редагування

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_OK_clicked();

private:
    Ui::Dialog_addCtr *ui;

    QString info__;     // змiнна тексту Дiалогу
    void *DataPoint__;  // змiнна вказiвник
};

#endif // DIALOG_ADDCTR_H
