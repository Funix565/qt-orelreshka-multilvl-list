#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog_add.h"
#include <types_kurs.h>
#include <functions_kurs.h>
#include "dialog_addctr.h"
#include "dialog_addeps.h"
#include "dialog_delzamfind.h"

#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

void **Start = InitArr(); // початкова iнiцiалiзацiя мультисписку

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Функцiї пошуку у комбiнованому списку
    ui->comboBox_search->addItem("Search for Continent (by name)");
    ui->comboBox_search->addItem("Search for Country (by name)");
    ui->comboBox_search->addItem("Search for Episode (by date)");

    ui->comboBox_search->insertSeparator(3);

    ui->comboBox_search->addItem("Search for the greatest prices on Continents");
    ui->comboBox_search->addItem("Country Rating by No. of Episodes");
    ui->comboBox_search->addItem("All Episodes where your Host has the Golden Card");

    // Зв'язок кнопок-континентiв на одну функцiю додавання
    connect(ui->pushButton_africa, SIGNAL(clicked()), this, SLOT(ElemAdd()));
    connect(ui->pushButton_asia, SIGNAL(clicked()), this, SLOT(ElemAdd()));
    connect(ui->pushButton_australia, SIGNAL(clicked()), this, SLOT(ElemAdd()));
    connect(ui->pushButton_europe, SIGNAL(clicked()), this, SLOT(ElemAdd()));
    connect(ui->pushButton_north, SIGNAL(clicked()), this, SLOT(ElemAdd()));
    connect(ui->pushButton_south, SIGNAL(clicked()), this, SLOT(ElemAdd()));

    // Зв'язок сигналу та функцiй видалення
    connect(ui->actionDel_eps, SIGNAL(triggered()), this, SLOT (ElemDel()));
    connect(ui->actionDel_ctr, SIGNAL(triggered()), this, SLOT (ElemDel()));
    connect(ui->actionDel_cont, SIGNAL(triggered()), this, SLOT (ElemDel()));

    // Зв'язок сигналу та функцiй редагування
    connect(ui->actionZam_cont, SIGNAL(triggered()), this, SLOT (ElemEdit()));
    connect(ui->actionZam_ctr, SIGNAL(triggered()), this, SLOT (ElemEdit()));
    connect(ui->actionZam_eps, SIGNAL(triggered()), this, SLOT (ElemEdit()));

    // Зв'язок кнопки континенту з Label
    connect(this, SIGNAL(set_label_cont (QString, QString)), this, SLOT(when_set_label(QString, QString)));

    // Додавання кореня
    if (((int*)Start[POS_CNT]) == 0)
    {
        int PosRoot = 0;
        void *PNew = new TRoot;
        ((TRoot*)PNew)->name = "Orel & Reshka";
        ((TRoot*)PNew)->ses_count = 25;
        ((TRoot*)PNew)->sublvl = InitArr();
        addToSort(Start, PNew, PosRoot);
    }

    // Налаштування вiдображення QTreeWidget
    ui->treeWidget_show->setHeaderHidden(true);
    ui->treeWidget_show->setColumnCount(1);
    ui->treeWidget_show->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->treeWidget_show->setColumnWidth(0, 800);

    ui->plainTextEdit->setReadOnly(true); // блокування введення тексту.

    // Ховаємо неосновнi компоненти
    ui->lineEdit_host->hide();
    ui->label_hostName->hide();
    ui->pushButton_findHost->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Процес додавання континенту
void MainWindow::ElemAdd()
{
    QPushButton *btn = (QPushButton *)sender(); // Отримання сигнала вiд кнопки

    if (btn->text()[0] != ' ') // якщо текст на кнопцi не дорiвнює пробiлу, отже, континент вже створений
    {
        QMessageBox::warning(this, "Error", "You can not add continent here. Try \"Edit Continent\" ");
        return;
    }

    Dialog_add diagCont(this);    // створення дiалогового вiкна

    emit update_activity("Adding Continent");    // назва вiкна

    diagCont.setModal(true);
    if (diagCont.exec() == QDialog::Accepted)   // якщо створено успiшно
    {
        void *Pnew = diagCont.retPrt(); // отримуємо данi
        int posFnd;
        bool isFound;
/* ContLvl*/        FindElList(((TRoot*)Start[0])->sublvl, ((TCont*)Pnew)->name, posFnd, isFound, compCont); // шукаємо мiсце для вставки
        if (isFound == true) // якщо континент вже iснує
        {
            QMessageBox::warning(this, "Error", "This Continent already exists"); // повiдомлення
        }
        else // додавання
        {
            addToSort(((TRoot*)Start[0])->sublvl, Pnew, posFnd);            // додаємо в масив вказiвникiв
            emit set_label_cont(((TCont*)Pnew)->name, btn->objectName());   // розмiщуємо назву на кнопцi

            btn->setText("");  // змiнюємо iм'я кнопки континенту
        }
        ShowAll(Start, ui);    // виведення
    }
}

// Процес додавання країни
void MainWindow::on_actionAdd_ctr_triggered()
{
    Dialog_addCtr diagCtr(this);              // створення дiалогового вiкна
    emit update_activity("Adding Country");   // назва вiкна
    diagCtr.setModal(true);

    if (diagCtr.exec() == QDialog::Accepted) // якщо створено успiшно
    {
        QString keyCont = diagCtr.retUp();  // отримуємо данi про верхнiй рiвень

        bool isFound;
        int posFndCont;
        FindElList(((TRoot*)Start[0])->sublvl, keyCont, posFndCont, isFound, compCont); // перевiряємо наявнiсть

        if (isFound == false)   // якщо не знайшли
        {
            QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
            ShowAll(Start, ui); // виведення
        }
        else // iнакше
        {
            void *Pnew = diagCtr.retPrt(); // отримуємо данi
            bool isFound;
            int posCtr;
/* CtrLvl*/             FindElList( ((TCont*)((TRoot*)Start[0])->sublvl[posFndCont])->sublvl, ((TCountry*)Pnew)->name, posCtr, isFound, compCountry );

            if (isFound == true)    // якщо країна вже iснує
            {
                QMessageBox::warning(this, "Error", "This Country already exists"); // повiдомлення
            }
            else // додавання
            {
                addToSort(((TCont*)((TRoot*)Start[0])->sublvl[posFndCont])->sublvl, Pnew, posCtr);
            }
        }
        ShowAll(Start, ui); // виведення
    }
}

// Процес додавання випуску
void MainWindow::on_actionAdd_eps_triggered()
{
    Dialog_addEps diagEps(this);            // створення дiалогового вiкна
    emit update_activity("Adding Episode");     // назва вiкна
    diagEps.setModal(true);

    if (diagEps.exec() == QDialog::Accepted) // якщо створено успiшно
    {
        QString keyCont = diagEps.retUpUp(); // данi про верхнiй рiвень
        bool isFound;
        int posFndCont;
        FindElList(((TRoot*)Start[0])->sublvl, keyCont, posFndCont, isFound, compCont); // пошук верхнього рiвня

        if (isFound == false)   // якщо не iснує
        {
            QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
            ShowAll(Start, ui); // виведення
        }

        QString keyCtr = diagEps.retUp();   // отримуємо данi верхнього рiвня
        int posFndCtr;
        FindElList( ((TCont*)((TRoot*)Start[0])->sublvl[posFndCont])->sublvl, keyCtr, posFndCtr, isFound, compCountry );
        if (isFound == false)   // якщо не знайшли
        {
            QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
            ShowAll(Start, ui);     // виведення
        }

        else
        {
            void *Pnew = diagEps.retPrt(); // отримуємо данi про рiвень, який додаємо
            int posEps;
            QString dateEps = ((TEpisode*)Pnew)->date.toString("dd/MM/yyyy");


 /* EpsLvl*/           FindElList(  ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posFndCont])->sublvl[posFndCtr])->sublvl, dateEps, posEps, isFound, compEps );

            if (isFound == true)    // якщо епiзод у цей день вже iснує
            {
                QMessageBox::warning(this, "Error", "This Episode already exists"); // повiдомлення
            }
            else
            {
                addToSort(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posFndCont])->sublvl[posFndCtr])->sublvl, Pnew, posEps); // додавання
            }
        }
        ShowAll(Start, ui); // виведення
    }
}

/* Процес видалення елементу */
void MainWindow::ElemDel()
{
    QAction *act = (QAction*)sender();            // отримуємо сигнал з Меню
    int ELEM_LEVEL;                               // на основi сигналу оголошуємо рiвень елемента
    if (act->objectName() == "actionDel_cont")
    {
        ELEM_LEVEL = 1;
    }
    if (act->objectName() == "actionDel_ctr")
    {
        ELEM_LEVEL = 2;
    }
    if (act->objectName() == "actionDel_eps")
    {
        ELEM_LEVEL = 3;
    }

    Dialog_DelZamFind diagDel(this);              // створення дiалогу для введення даних
    emit update_activity("Removing Element");     // заголовок вiкна
    emit set_rules();                             // виведення iнформацiї
    emit set_level(ELEM_LEVEL);                   // передаємо рiвень елемента
    diagDel.setModal(true);

    if (diagDel.exec() == QDialog::Accepted)      // якщо створено успiшно
    {
        QString Cont, Ctr, EpsDate;
        diagDel.retLevels(Cont, Ctr, EpsDate);    // отримуємо данi про верхнi рiвнi

        if (Cont.length() != 0 && Ctr.length() != 0 && EpsDate.length() != 0) // видаляємо випуск
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont);   // шукаємо верхнiй рiвень Континент
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);   // шукаємо рiвень Країна
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posDate;
            FindElList(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl, EpsDate, posDate, isFound, compEps); // шукаємо випуск
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Episode was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            void *delEl = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posDate]); // отримуємо доступ до елемента
            delete (TEpisode*)delEl;    // звiльняємо пам'ять
            DelElList(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl, posDate); // робимо зсув у масивi вказiвникiв

            ShowAll(Start, ui); // виведення
        }
        else if (Cont.length() != 0 && Ctr.length() != 0)  // видаляємо країну
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont); // шукаємо верхнiй рiвень
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);  // шукаємо Країну
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            delEpsLvl(  ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl);       // видаляємо пiдрiвень епiзодiв

            void *delEl = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr]);              // отримуємо доступ до елемента
            delete (TCountry*)delEl;    // звiльняєм пам'ять

            DelElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, posCtr);   // робимо зсув у масивi вказiвникiв
            ShowAll(Start, ui); // виведення
        }
        else if (Cont.length() != 0)        // видаляємо континент
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont);   // пошук континенту
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            delCtryLvl( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl );      // видалення пiдрiвня. (I там передбачено видалення ще одного пiдрiвня)
            void *delEl = (((TRoot*)Start[0])->sublvl)[posCont];                      // отримуємо доступ до елемента
            QString oldName = ((TCont*)delEl)->name;
            delete (TCont*)delEl;                                                     // видаляємо елемент
            DelElList( ((TRoot*)Start[0])->sublvl, posCont);                          // зсув у масивi вказiвникiв

            DelEdit_label (ui, " -----", oldName);  // змiна назви на кнопцi
            ShowAll(Start, ui); // виведення
        }
    }
}

/* Ф-цiя редагування даних про елемент*/
void MainWindow::ElemEdit()
{
    QAction *act = (QAction*)sender();            // отримуємо сигнал з Меню
    int ELEM_LEVEL;                               // на основi сигналу оголошуємо рiвень елемента
    if (act->objectName() == "actionZam_cont")
    {
        ELEM_LEVEL = 1;
    }
    if (act->objectName() == "actionZam_ctr")
    {
        ELEM_LEVEL = 2;
    }
    if (act->objectName() == "actionZam_eps")
    {
        ELEM_LEVEL = 3;
    }

    Dialog_DelZamFind diagEdit(this); // дiалог пошуку
    emit update_activity("Search for Editing Element");
    emit set_rules();
    emit set_level(ELEM_LEVEL);
    diagEdit.setModal(true);

    if (diagEdit.exec() == QDialog::Accepted)
    {
        QString Cont, Ctr, EpsDate;
        diagEdit.retLevels(Cont, Ctr, EpsDate); // отримуємо данi про верхнi рiвнi

        // Редагуємо випуск
        if (Cont.length() != 0 && Ctr.length() != 0 && EpsDate.length() != 0)
        {
            int posCont;
            bool isFound;
            FindElList( ((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont );
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);   // шукаємо рiвень Країна
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posDate;
            FindElList(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl, EpsDate, posDate, isFound, compEps); // шукаємо випуск
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Episode was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            void *OldData = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posDate]); // отримуємо доступ до елемента

            Dialog_addEps diagEditEps(this);     // дiалог редагування
            emit update_activity("Editing Episode");
            emit set_data_forEdit(OldData);
            diagEditEps.setModal(true);

            if (diagEditEps.exec() == QDialog::Accepted) // якщо НЕ скасовано
            {
                void *PEdit = diagEditEps.retEdit(OldData);     // отримуємо елемент
                QString keyDate = ((TEpisode*)PEdit)->date.toString("dd/MM/yyyy");  // ключове поле
                // ф-цiя впорядкування
                zamInSort( ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl, keyDate, posDate, compEps);
                ShowAll(Start, ui);
            }
        }
        // Редагуємо країну
        else if (Cont.length() != 0 && Ctr.length() != 0)
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont); // шукаємо верхнiй рiвень
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);  // шукаємо Країну
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }
            void *OldData = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr]);

            Dialog_addCtr diagEditCtr(this);    // дiалог редагування
            emit update_activity("Editing Country");
            emit set_data_forEdit(OldData);
            diagEditCtr.setModal(true);

            if (diagEditCtr.exec() == QDialog::Accepted)
            {
                void *PEdit = diagEditCtr.retEdit(OldData); // елемент
                QString keyCtr = ((TCountry*)PEdit)->name;  // ключове поле
                // впорядкування
                zamInSort( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, keyCtr, posCtr, compCountry );
                ShowAll(Start, ui);
            }
        }
        // Редагування континенту
        else if (Cont.length() != 0)
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont); // шукаємо верхнiй рiвень
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }


            void *OldData = (((TRoot*)Start[0])->sublvl)[posCont]; // елемент
            QString oldName = ((TCont*)OldData)->name;  // стара назва кнопки для пошуку

            Dialog_add diagEditCont (this); // дiалог редагування
            emit update_activity("Editing Continent");
            emit set_data_forEdit(OldData);
            diagEditCont.setModal(true);

            if (diagEditCont.exec() == QDialog::Accepted)
            {
                void *PEdit = diagEditCont.retEdit(OldData); //елемент
                QString keyCont = ((TCont*)PEdit)->name;     // ключове поле
                // впорядкування
                zamInSort( ((TRoot*)Start[0])->sublvl, keyCont, posCont, compCont );

                DelEdit_label(ui, keyCont, oldName); // змiна тексту на кнопцi
                ShowAll(Start, ui);
            }
        }
    }
}

// Ф-цiя змiни тексту на кнопцi Континент
void MainWindow::DelEdit_label(Ui::MainWindow *ui, QString newName, QString oldName)
{
    QString state = "";
    if (newName == " -----") // якщо iм'я - ознака видалення
    {
        state = " .";   // нове iм'я континенту - порожнє
    }

    if (ui->label_africa->text() == oldName)
    {
        ui->label_africa->setText(newName);
        ui->pushButton_africa->setText(state);

        return;
    }
    if (ui->label_asia->text() == oldName)
    {
        ui->label_asia->setText(newName);
        ui->pushButton_asia->setText(state);
        return;
    }
    if (ui->label_australia->text() == oldName)
    {
        ui->label_australia->setText(newName);
        ui->pushButton_australia->setText(state);
        return;
    }
    if (ui->label_europe->text() == oldName)
    {
        ui->label_europe->setText(newName);
        ui->pushButton_europe->setText(state);
        return;
    }
    if (ui->label_north->text() == oldName)
    {
        ui->label_north->setText(newName);
        ui->pushButton_north->setText(state);
        return;
    }
    if (ui->label_south->text() == oldName)
    {
        ui->label_south->setText(newName);
        ui->pushButton_south->setText(state);
        return;
    }
}

/* Ф-цiя встановлення назви на континентi */
void MainWindow::when_set_label(QString nameCont, QString btnText)
{
    nameCont__ = nameCont;
    btnText__ = btnText;

    if (btnText__ == ui->pushButton_africa->objectName())
    {
        ui->label_africa->setText(nameCont__);
        return;
    }
    if (btnText__ == ui->pushButton_asia->objectName())
    {
        ui->label_asia->setText(nameCont__);
        return;
    }
    if (btnText__ == ui->pushButton_australia->objectName())
    {
        ui->label_australia->setText(nameCont__);
        return;
    }
    if (btnText__ == ui->pushButton_europe->objectName())
    {
        ui->label_europe->setText(nameCont__);
        return;
    }
    if (btnText__ == ui->pushButton_north->objectName())
    {
        ui->label_north->setText(nameCont__);
        return;
    }
    if (btnText__ == ui->pushButton_south->objectName())
    {
        ui->label_south->setText(nameCont__);
        return;
    }
}

// Обробка функцiй пошуку у комбiнованому списку
void MainWindow::on_pushButton_find_clicked()
{
    int ELEM_LEVEL;
    int func = ui->comboBox_search->currentIndex(); // аналiзуємо вибiр користувача
    // вставновлюємо рiвень елемента для отримання необхiдних даних
    switch (func)
    {
    case 0:
        {
            ELEM_LEVEL = 1;
        }
        break;
    case 1:
        {
            ELEM_LEVEL = 2;
        }
        break;
    case 2:
        {
            ELEM_LEVEL = 3;
        }
        break;
    case 3:
        {
            MinMaxSum(Start, ui);    // мiнiмальна/максимальна сума випуску на континентi
            return;
        }
        break;
    case 4:
        {
            CtrRate(Start, ui);     // Рейтинг за к-сть випускiв
            return;
        }
        break;
    case 5:
        {
            // показуємо додатковi елементи iнтерфейсу
            ui->lineEdit_host->show();
            ui->lineEdit_host->clear();
            ui->label_hostName->show();
            ui->pushButton_findHost->show();
            return;
        }
        break;
    }

    Dialog_DelZamFind diagFind(this);           // дiалог пошуку
    emit update_activity("Search for Element");
    emit set_rules();
    emit set_level(ELEM_LEVEL);
    diagFind.setModal(true);

    // якщо успiшно
    if (diagFind.exec() == QDialog::Accepted)
    {
        QString Cont, Ctr, EpsDate;
        diagFind.retLevels(Cont, Ctr, EpsDate);

        if (Cont.length() != 0 && Ctr.length() != 0 && EpsDate.length() != 0) // шукємо випуск
        {
            int posCont;
            bool isFound;
            FindElList( ((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont );
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);   // шукаємо рiвень Країна
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posDate;
            FindElList(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl, EpsDate, posDate, isFound, compEps); // шукаємо випуск
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Episode was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            // Отримали iндекс континенту, країни та випуску
            SearchEps (Start, ui, posCont, posCtr, posDate); // ф-цiя виведення знайдених даних
        }
        else if (Cont.length() != 0 && Ctr.length() != 0) // шукаємо країну
        {
            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont); // шукаємо верхнiй рiвень
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            int posCtr;
            FindElList(((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl, Ctr, posCtr, isFound, compCountry);  // шукаємо Країну
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Country was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }
            // Отримали iндекс континенту та країни
            SearchCtry(Start, ui, posCont, posCtr); // ф-цiя виведення
        }
        else if (Cont.length() != 0)    // шукаємо континент
        {
            qDebug() << "cont finding";
            qDebug() << "Cont == " << Cont;

            int posCont;
            bool isFound;
            FindElList(((TRoot*)Start[0])->sublvl, Cont, posCont, isFound, compCont); // шукаємо верхнiй рiвень
            if (isFound == false)
            {
                QMessageBox::warning(this, "Error", "Continent was not found"); // повiдомлення
                ShowAll(Start, ui); // виведення
                return;
            }

            SearchCont(Start, ui, posCont); // виведення даних
        }
    }
}

// Обробка сигналу вiд натискання кнопки пошуку ведучого для iндивiдальної функцiї
void MainWindow::on_pushButton_findHost_clicked()
{
    QString keyHost = ui->lineEdit_host->text();    // отримуємо ключове поле ведучого

    // Якщо порожнє або не введене
    if (keyHost.length() == 0 || keyHost[0] == ' ')
    {
        QMessageBox::information(this, "Error", "Enter name and try again"); // повiдомлення
    }
    else
    {
        FindHost(Start, ui, keyHost); // виклик функцiї пошуку
    }

    // Ховаємо непотрiбнi елементи iнтерфейсу
    ui->lineEdit_host->hide();
    ui->pushButton_findHost->hide();
    ui->label_hostName->hide();
}

// Процес збереження мультисписку у файл
void MainWindow::on_actionSave_triggered()
{
    QFileDialog diagFile;
    diagFile.setAcceptMode(QFileDialog::AcceptSave); // дiалог для збереження файлу
    diagFile.setFileMode(QFileDialog::AnyFile);      // доступнi файли для вибору
    diagFile.setNameFilter(".txt");                  // фiльтр для текстових файлiв

    // Якщо не погодилися
    if (diagFile.exec() != QDialog::Accepted)
    {
        return; // вихiд
    }

    QString fileName = diagFile.selectedFiles().at(0); // змiнна iм'я файлу. отримує шлях до вибраного файлу

    // Доповнення розширенням
    if (!fileName.endsWith(".txt"))
    {
        fileName += ".txt";
    }
    saveINFile(Start, fileName);    // виклик функцiї збереження
}

/* Ф-цiя створення списку з файлу */
void MainWindow::on_actionOpen_triggered()
{
    QFileDialog diagFileOp;
    diagFileOp.setAcceptMode(QFileDialog::AcceptOpen);  // режим дiалогу - вiдкриття
    diagFileOp.setFileMode(QFileDialog::ExistingFile);  // тiльки файли, шо iснують
    diagFileOp.setNameFilter(".txt");                   // фiльтр для текстових файлiв

    // Якщо не погодилися
    if (diagFileOp.exec() != QDialog::Accepted)
    {
        return; // вихiд
    }

    QString fileName = diagFileOp.selectedFiles().at(0);  // отримуємо шлях до файлу
    // Доповнення розширенням
    if (!fileName.endsWith(".txt"))
    {
        fileName += ".txt";
    }
    openFROMFile(Start, fileName); // виклик функцiї збереження
    update();
    ShowAll(Start, ui); // виведення
}
