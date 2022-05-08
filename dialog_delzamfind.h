#ifndef DIALOG_DELZAMFIND_H
#define DIALOG_DELZAMFIND_H

#include <QDialog>

namespace Ui {
class Dialog_DelZamFind;
}

class Dialog_DelZamFind : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_DelZamFind(QWidget *parent = 0);
    ~Dialog_DelZamFind();

    void retLevels (QString &Cont, QString &Ctr, QString &Eps); // повернення ключових полiв рiвнiв

public slots:
    void when_update_activity (QString info);   // текст у дiалозi
    void when_set_rules ();                     // iнформацiя по введенню даних
    void when_set_level(int level);             // змiна рiвня, з яким вiдбувається операцiя

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_Cancel_clicked();

private:
    Ui::Dialog_DelZamFind *ui;

    QString info__;     // змiнна тексту Дiалогу
    int level__;        // рiвень елемента
};

#endif // DIALOG_DELZAMFIND_H
