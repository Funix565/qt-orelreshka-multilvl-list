#include "dialog_add.h"
#include "ui_dialog_add.h"
#include <types_kurs.h>
#include <functions_kurs.h>
#include <QMessageBox>
#include <QDebug>

Dialog_add::Dialog_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_add)
{
    ui->setupUi(this);

    connect(parent, SIGNAL(update_activity(QString)), this, SLOT(when_update_activity(QString)));   // змiна заголовку
    connect(parent, SIGNAL(set_data_forEdit(void *)), this, SLOT(when_set_data_forEdit(void *)));   // виведення даних для редагування
}

Dialog_add::~Dialog_add()
{
    delete ui;
}

/* Ф-цiя виведення даних для замiни*/
void Dialog_add::when_set_data_forEdit(void *DataPoint)
{
    DataPoint__ = DataPoint;
    QString nameC = ((TCont*)DataPoint__)->name;
    QString landM = QString::number(((TCont*)DataPoint__)->landmass, 'f', 2);
    QString sqr = QString::number(((TCont*)DataPoint__)->square);

    ui->lineEdit_landmassCont->setText(landM);
    ui->lineEdit_nameCont->setText(nameC);
    ui->lineEdit_sqrCont->setText(sqr);
}

// Оновлення заголовку вiкна
void Dialog_add::when_update_activity(QString info)
{
    info__ = info;
    ui->label_activity->setText(info__);    // функцiя змiни заголовку
}

// Скасувати
void Dialog_add::on_pushButton_cancel_clicked()
{
    close();
}

// Перевiрка на коректнiсть пiсля натискання кнопки ОК
void Dialog_add::on_pushButton_ok_clicked()
{
    bool correct = true;                            // змiнна для перевiрки коректностi вводу
    QString name = ui->lineEdit_nameCont->text();   // отримуємо назву континенту
    if (name.length() == 0 || name[0] == ' ')       // якщо нiчого не введено
    {
        QMessageBox::information(this, "Error", "Enter name or quit"); // повiдомлення
        correct = false;    // коректнiсть - false
    }

    bool ok;
    unsigned int sqr = ui->lineEdit_sqrCont->text().toUInt(&ok, 10);    // отримуємо площу
    if (sqr == 0) // якщо площа нуль
    {
        QMessageBox::information(this, "Error", "Invalid square");      // повiдомлення
        correct = false;
    }

    double landmass = ui->lineEdit_landmassCont->text().toDouble(&ok);  // отримуємо вiдсоток сушi
    if (landmass <= 0 || landmass > 33) // якщо неправильно
    {
        QMessageBox::information(this, "Error", "Invalid landmass");    // повiдомлення
        correct = false;
    }

    if (correct) // якщо всi данi коректнi
    {
        accept(); // дiалог закривається успiшно
    }
}

/* Ф-цiя повернення вказiвника зi змiненими даними*/
void *Dialog_add::retEdit(void *DataPoint) const
{
    bool ok;
    QString name, str;

    name = ui->lineEdit_nameCont->text();
    str = ui->lineEdit_sqrCont->text();
    unsigned int sqr = str.toInt(&ok, 10);

    str = ui->lineEdit_landmassCont->text();
    double landmass = str.toDouble(&ok);

    ((TCont*)DataPoint)->name = name;
    ((TCont*)DataPoint)->landmass = landmass;
    ((TCont*)DataPoint)->square = sqr;

    return DataPoint;
}

void* Dialog_add::retPrt() const    // функцiя отримання даних
{
    bool ok;
    QString name;
    QString str;

    name = ui->lineEdit_nameCont->text();   // отримуємо iм'я
    str = ui->lineEdit_sqrCont->text();     // отримуємо площу
    unsigned int sqr = str.toUInt(&ok, 10);

    str = ui->lineEdit_landmassCont->text(); // отримуємо вiдсоток сушi
    double landmass = str.toDouble(&ok);

    void *Pnew = new TCont;      // створюємо вказiвник
    ((TCont*)Pnew)->name = name; //запомнюємо поля
    ((TCont*)Pnew)->square = sqr;
    ((TCont*)Pnew)->landmass = landmass;
    ((TCont*)Pnew)->sublvl = InitArr();

    return Pnew;    // повертаємо
}
