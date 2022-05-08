#include "dialog_delzamfind.h"
#include "ui_dialog_delzamfind.h"
#include <types_kurs.h>
#include <QMessageBox>

Dialog_DelZamFind::Dialog_DelZamFind(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_DelZamFind)
{

    ui->setupUi(this);

    connect(parent, SIGNAL(update_activity(QString)), this, SLOT(when_update_activity(QString))); // сигнал змiни заголовку дiалогу
    connect(parent, SIGNAL(set_rules()), this, SLOT (when_set_rules()));                          // виведення ?нформац?ї по введенню даних
    connect(parent, SIGNAL(set_level(int)), this, SLOT (when_set_level(int)));                    // отримання р?вня елемента
}

Dialog_DelZamFind::~Dialog_DelZamFind()
{
    delete ui;
}

// Виведення iнформацiї про операцiю
void Dialog_DelZamFind::when_update_activity(QString info)
{
    info__ = info;
    ui->label_act->setText(info__);
}

// Виведення допомiжних правил
void Dialog_DelZamFind::when_set_rules()
{
    ui->label_info->setTextFormat(Qt::RichText);
    ui->label_info->setText("<html><head/><body><p align=\"center\"><span style=\" font-weight:600;\">Info</span></p><p>If you want to delete/find Continent - fill in the first line;</p><p>If you want to delete/find Country - fill in the first and the second lines;</p><p>If you want to delete/find Episode - fill in all lines.</p></body></html>");
}

// Приймає рiвень елемента
void Dialog_DelZamFind::when_set_level(int level)
{
    level__ = level;
}

/* Ф-цiя повернення введених даних */
void Dialog_DelZamFind::retLevels(QString &Cont, QString &Ctr, QString &Eps)
{
    if (level__ == 1)
    {
        Cont = ui->lineEdit_Cont->text();   // повертаємо континент
    }
    if (level__ == 2)
    {
        Cont = ui->lineEdit_Cont->text();   // повертаємо континент
        Ctr = ui->lineEdit_Ctr->text();     // повертаємо країну
    }
    if (level__ == 3)
    {
        Cont = ui->lineEdit_Cont->text();   // повертаємо континент
        Ctr = ui->lineEdit_Ctr->text();     // повертаємо країну
        Eps = ui->lineEdit_Eps->text();     // повертаємо епiзод
    }
}

void Dialog_DelZamFind::on_pushButton_ok_clicked() // Перевiрка даних на коректнiсть
{
    bool correct = true;
    // Якщо видаляємо третiй рiвень - потрiбно перевiрити всi поля
    if (level__ == 3)
    {
        QString Cont = ui->lineEdit_Cont->text();
        if (Cont.length() == 0 || Cont[0] == ' ')
        {
            QMessageBox::information(this, "Error", "Fill in upper level or Cancel");
            correct = false;
        }
        QString Ctr = ui->lineEdit_Ctr->text();
        if (Ctr.length() == 0 || Ctr[0] == ' ')
        {
            QMessageBox::information(this, "Error", "Fill in upper level or Cancel");
            correct = false;
        }
        QString date = ui->lineEdit_Eps->text();
        QDate dateEps = QDate::fromString(date, "dd/MM/yyyy");
        if (dateEps.isValid() == false)
        {
            QMessageBox::information(this, "Error", "Invalid date");
            correct = false;
        }
    }
    // Якщо видаляємо другий рiвень
    else if (level__ == 2)
    {
        QString Cont = ui->lineEdit_Cont->text();
        if (Cont.length() == 0 || Cont[0] == ' ')  // Перевiярємо континент
        {
            QMessageBox::information(this, "Error", "Fill in upper level or Cancel");
            correct = false;
        }
        QString Ctr = ui->lineEdit_Ctr->text();
        if (Ctr.length() == 0 || Ctr[0] == ' ')    // Перевiярємо країну
        {
            QMessageBox::information(this, "Error", "Fill in upper level or Cancel");
            correct = false;
        }
    }
    // Якщо видаляємо перший рiвень
    else if (level__ == 1)
    {
        QString Cont = ui->lineEdit_Cont->text();
        if (Cont.length() == 0 || Cont[0] == ' ')  // Перевiярємо континент
        {
            QMessageBox::information(this, "Error", "Fill in upper level or Cancel");
            correct = false;
        }
    }

    if (correct)
    {
        accept();
    }
}

// Скасування
void Dialog_DelZamFind::on_pushButton_Cancel_clicked()
{
    close();
}
