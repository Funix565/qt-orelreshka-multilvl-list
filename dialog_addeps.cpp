#include "dialog_addeps.h"
#include "ui_dialog_addeps.h"
#include <functions_kurs.h>
#include <QDate>
#include <QMessageBox>
#include <QDebug>

Dialog_addEps::Dialog_addEps(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_addEps)
{
    ui->setupUi(this);

    connect(parent, SIGNAL(update_activity(QString)), this, SLOT(when_update_activity(QString))); // сигнал змiни заголовку дiалогу
    connect(parent, SIGNAL(set_data_forEdit(void *)), this, SLOT(when_set_data_forEdit(void*)));  // сигнал заповнення полiв для змiнення
}

Dialog_addEps::~Dialog_addEps()
{
    delete ui;
}

/* Ф-цiя заповнення полiв для редагування*/
void Dialog_addEps::when_set_data_forEdit(void *DataPoint)
{
    // Ховаємо верхнi рiвнi
    ui->lineEdit_contName->hide();
    ui->label_upCont->hide();
    ui->lineEdit_ctrName->hide();
    ui->label_upCtr->hide();

    // Отримуємо дан?
    DataPoint__ = DataPoint;
    QDate date = ((TEpisode*)DataPoint__)->date;
    QString dateEps = date.toString("dd/MM/yyyy");
    QString cityEps = ((TEpisode*)DataPoint__)->city;
    QString fio1 = ((TEpisode*)DataPoint__)->fio1;
    QString fio2 = ((TEpisode*)DataPoint__)->fio2;
    QString SpentSum = QString::number(((TEpisode*)DataPoint__)->SpentSum, 'f', 2);

    // Пом?щаємо у поля
    ui->lineEdit_city->setText(cityEps);
    ui->lineEdit_date->setText(dateEps);
    ui->lineEdit_fio1->setText(fio1);
    ui->lineEdit_fio2->setText(fio2);
    ui->lineEdit_summa->setText(SpentSum);
}

// Функцiя змiни заголовку
void Dialog_addEps::when_update_activity(QString info)
{
    info__ = info;
    ui->label_activity->setText(info__);
}

/* Ф-цiя повернення вказiвника зi змiненими даними*/
void *Dialog_addEps::retEdit(void *DataPoint) const
{
    bool ok;
    QString date, city, fio1, fio2, str;
    bool card1, card2;

    date = ui->lineEdit_date->text();   // отримуємо дату
    QDate dateEps = QDate::fromString(date, "dd/MM/yyyy");

    city = ui->lineEdit_city->text();   // мiсто
    str = ui->lineEdit_summa->text();
    double Sum = str.toDouble(&ok);     // витрачену суму

    fio1 = ui->lineEdit_fio1->text();   // ПiБ першого ведучого
    fio2 = ui->lineEdit_fio2->text();   // ПiБ другого

    // Наявнiсть золотої карти
    if (ui->radioButton_card1->isChecked() == true)
    {
        card1 = true;
        card2 = false;
    }
    else
    {
        card1 = false;
        card2 = true;
    }

    ((TEpisode*)DataPoint)->date = dateEps;
    ((TEpisode*)DataPoint)->city = city;
    ((TEpisode*)DataPoint)->SpentSum = Sum;
    ((TEpisode*)DataPoint)->fio1 = fio1;
    ((TEpisode*)DataPoint)->fio2 = fio2;
    ((TEpisode*)DataPoint)->card1 = card1;
    ((TEpisode*)DataPoint)->card2 = card2;

    return DataPoint;
}

// Ф-цiя отримання даних
void *Dialog_addEps::retPrt() const
{
    bool ok;
    QString date;
    QString city;
    QString fio1, fio2;
    bool card1, card2;
    QString str;

    date = ui->lineEdit_date->text();   // отримуємо дату
    QDate dateEps = QDate::fromString(date, "dd/MM/yyyy");

    city = ui->lineEdit_city->text();   // мiсто
    str = ui->lineEdit_summa->text();
    double Sum = str.toDouble(&ok);     // витрачену суму

    fio1 = ui->lineEdit_fio1->text();   // ПiБ першого ведучого
    fio2 = ui->lineEdit_fio2->text();   // ПiБ другого

    // Наявнiсть золотої карти
    if (ui->radioButton_card1->isChecked() == true)
    {
        card1 = true;
        card2 = false;
    }
    else
    {
        card1 = false;
        card2 = true;
    }

    void *Pnew = new TEpisode;  // створюємо вказiвник
    // Заносимо данi
    ((TEpisode*)Pnew)->date = dateEps;
    ((TEpisode*)Pnew)->city = city;
    ((TEpisode*)Pnew)->SpentSum = Sum;
    ((TEpisode*)Pnew)->fio1 = fio1;
    ((TEpisode*)Pnew)->fio2 = fio2;
    ((TEpisode*)Pnew)->card1 = card1;
    ((TEpisode*)Pnew)->card2 = card2;

    return Pnew;    // повертаємо
}

// Повернення верхнiх пiдрiвнiв
QString Dialog_addEps::retUpUp() const
{
    return ui->lineEdit_contName->text();
}

// Повернення верхнiх пiдрiвнiв
QString Dialog_addEps::retUp() const
{
    return ui->lineEdit_ctrName->text();
}

// Скасування
void Dialog_addEps::on_pushButton_cancel_clicked()
{
    close();
}

// Перевiрка
void Dialog_addEps::on_pushButton_OK_clicked()
{
    bool correct = true;

    QString up1, up2;
    up1 = ui->lineEdit_contName->text();
    up2 = ui->lineEdit_ctrName->text();

    // Якщо не редагування, потр?бно отримати верхн? р?вн?
    if (ui->label_activity->text() != "Editing Episode")
    {

        if (up1.length() == 0 || up1[0] == ' ' || up2.length() == 0 || up2[0] == ' ')
        {
            QMessageBox::warning(this, "Error", "Fill in upper levels");
            correct = false;
        }
    }

    QString date = ui->lineEdit_date->text();
    QDate dateEps = QDate::fromString(date, "dd/MM/yyyy");
    if (dateEps.isNull())
    {
        QMessageBox::information(this, "Error", "Invalid Date");    // пов?домлення
        correct = false;
    }

    QString city = ui->lineEdit_city->text();
    if (city.length() == 0 || city[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter city or Cancel"); // пов?домлення
        correct = false;
    }

    bool ok;
    double sum = ui->lineEdit_summa->text().toDouble(&ok);
    if (sum <= 0)
    {
        QMessageBox::information(this, "Error", "Invalid sum");   // пов?домлення
        correct = false;
    }

    QString fio, fio2;
    fio = ui->lineEdit_fio1->text();
    if (fio.length() == 0 || fio[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter fio or Cancel"); // повiдомлення
        correct = false;
    }

    fio2 = ui->lineEdit_fio2->text();
    if (fio2.length() == 0 || fio2[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter fio or Cancel");  // повiдомлення
        correct = false;
    }
    // Ведучi повиннi бути рiзними
    if (fio == fio2)
    {
        QMessageBox::information(this, "Error", "Hosts have to be different");  // повiдомлення
        correct = false;
    }

    if (ui->radioButton_card1->isChecked() == false && ui->radioButton_card2->isChecked() == false)
    {
        QMessageBox::information(this, "Error", "Choose card");  // пов?домлення
        correct = false;
    }

    if (correct)
    {
        accept();
    }
}
