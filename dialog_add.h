#ifndef DIALOG_ADD_H
#define DIALOG_ADD_H

#include <QDialog>

namespace Ui {
class Dialog_add;
}

class Dialog_add : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_add(QWidget *parent = 0);
    ~Dialog_add();

    void* retPrt() const; // повренення вказiвник з даними
    void* retEdit(void* DataPoint) const;   // повернення вказiвника з вiдредагованими даними

public slots:
    void when_update_activity (QString info);       // текст у дiалозi
    void when_set_data_forEdit (void *DataPoint);   // передача вказiвника для заповнення полiв на редагування

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_add *ui;

    QString info__;         // змiнна тексту Дiалогу
    void *DataPoint__;      // змiнна вказiвник
};

#endif // DIALOG_ADD_H
