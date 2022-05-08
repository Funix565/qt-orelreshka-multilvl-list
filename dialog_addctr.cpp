#include "dialog_addctr.h"
#include "ui_dialog_addctr.h"
#include <functions_kurs.h>
#include <QString>
#include <QMessageBox>
#include <QDebug>

Dialog_addCtr::Dialog_addCtr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_addCtr)
{
    ui->setupUi(this);

    connect(parent, SIGNAL(update_activity(QString)), this, SLOT(when_update_activity(QString)));   // сигнал для змiни заголовку
    connect(parent, SIGNAL(set_data_forEdit(void *)), this, SLOT(when_set_data_forEdit(void*)));    // сигнал заповнення полiв для змiнення
}

Dialog_addCtr::~Dialog_addCtr()
{
    delete ui;
}

/* Ф-цiя заповнення полiв для редагування*/
void Dialog_addCtr::when_set_data_forEdit(void *DataPoint)
{
    // Ховаємо верхнi рiвнi
    ui->lineEdit_nameContUp->hide();
    ui->label_UpCont->hide();

    // Отримуємо дан?
    DataPoint__ = DataPoint;
    QString capName = ((TCountry*)DataPoint__)->capital;
    QString ctrName = ((TCountry*)DataPoint__)->name;
    QString numEps = QString::number(((TCountry*)DataPoint__)->numOfEpisodes);

    // Пом?щаємо у поля
    ui->lineEdit_nameCap->setText(capName);
    ui->lineEdit_nameCtr->setText(ctrName);
    ui->lineEdit_numsEps->setText(numEps);
}

// Функцiя змiни заголовку
void Dialog_addCtr::when_update_activity(QString info)
{
    info__ = info;
    ui->label_activity->setText(info__);
}

/* Ф-цiя повернення змiнених даних */
void* Dialog_addCtr::retEdit(void *DataPoint) const
{
    bool ok;
    QString name, capital, str;
    name = ui->lineEdit_nameCtr->text();
    capital = ui->lineEdit_nameCap->text();
    str = ui->lineEdit_numsEps->text();
    int NumEps = str.toInt(&ok, 10);
    ((TCountry*)DataPoint)->name = name;
    ((TCountry*)DataPoint)->capital = capital;
    ((TCountry*)DataPoint)->numOfEpisodes = NumEps;

    return DataPoint;
}

void* Dialog_addCtr::retPrt() const    // функцiя отримання даних
{
    bool ok;
    QString name;
    QString capital;
    QString str;

    name = ui->lineEdit_nameCtr->text();    // отримуємо iм'я
    capital = ui->lineEdit_nameCap->text(); // отримуємо столицю
    str = ui->lineEdit_numsEps->text();     // отримуємо к-сть випускiв
    int NumEps = str.toInt(&ok, 10);

    void *Pnew = new TCountry;              // створюємо вказiвник
    ((TCountry*)Pnew)->name = name;         //заповнюємо поля
    ((TCountry*)Pnew)->capital = capital;
    ((TCountry*)Pnew)->numOfEpisodes = NumEps;
    ((TCountry*)Pnew)->sublvl = InitArr();

    return Pnew;    // повертаємо
}

// Повернення верхнього рiвня
QString Dialog_addCtr::retUp() const
{
    return ui->lineEdit_nameContUp->text();
}

// Скасування
void Dialog_addCtr::on_pushButton_cancel_clicked()
{
    close();
}

// Перевiрка правильностi пiсля натискання на кнопку ОК
void Dialog_addCtr::on_pushButton_OK_clicked()
{
    bool correct = true;

    // Якщо не редагуємо, по потр?бн? дан? про верхн?й р?вень
    if (ui->label_activity->text() != "Editing Country")
    {
        QString up1 = ui->lineEdit_nameContUp->text();
        if (up1.length() == 0 || up1[0] == ' ')
        {
            QMessageBox::warning(this, "Error", "Fill in upper level");
            correct = false;
        }
    }

    QString name = ui->lineEdit_nameCtr->text();
    if (name.length() == 0 || name[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter name or quit");
        correct = false;
    }

    QString cap = ui->lineEdit_nameCap->text();
    if (cap.length() == 0 || cap[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter capital or quit");
        correct = false;
    }

    bool ok;
    int numEps = ui->lineEdit_numsEps->text().toInt(&ok, 10);
    if (numEps <= 0)
    {
        QMessageBox::information(this, "Error", "Invalid nums");
        correct = false;
    }

    if (correct)
    {
        accept();
    }
}
