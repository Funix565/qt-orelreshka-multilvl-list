#include <types_kurs.h>
#include <functions_kurs.h>
#include <QDebug>

/* Ф-цiя iнiцiалiзацiї.
   Повертає адресу першого вказiвника в масивi вказiвникiв */
void** InitArr()
{
    /* SIZE_AR_FIRST -  константа, початковий розмiр масиву*/
    void** ptr = new void* [SIZE_ARR_FIRST + 2]; // видiлення пам'ятi для нового масиву вказiвникiв
    ptr += 2;                                    // змiщення вказiвника на перший елемент. Тепер вiн вказує на третiй елемент
    ((int*)ptr)[POS_CNT] = 0;                // службовий елемент - к-сть структур у списку
    ((int*)ptr)[POS_SZ] = SIZE_ARR_FIRST;    // службовий елемент - допустима к-сть структур

    return ptr;
}
/* Ф-цiя розширення масиву вказiвникiв.
   Приймає вказiвник на масив вказiвникiв, розмiр, к-сть елементiв */
void ExpandArrPtr(void** &Start, int &SizeArr, int Cnt)
{
        int sizeOld = SizeArr;      // запам'ятовуємо старий розмiр для циклу копiювання
        SizeArr = SizeArr + DELTA;  // новий розмiр. Збiльшення на DELTA.
        void **newArr = new void* [SizeArr + 2];    // Видiлення пам'ятi для нового маисву вказiвникiв бiльшого розмiру

        // Цикл копiювання старого масиву у новий
        for (int i = 0; i < sizeOld; i++)
        {
            newArr[i + 2] = Start[i];
        }
        Start = Start - 2;  // змiщення вказiника на початок
        delete []Start;   // звiльнення пам'ятi
        Start = newArr + 2;   // присвоєння нової адреси та змiщення вказiвника на перший елемент

        // Занесення кiлькостi та допустимого ромiру у новий масив
        ((int*)Start)[POS_CNT] = Cnt;
        ((int*)Start)[POS_SZ] = SizeArr;
}

/* Ф-цiя додавання вказiвника у вiдсортований масив вказiвникiв
   Приймає вказ. на початок, новий вказiвник, позицiю для вставки.*/
void addToSort(void** &Start, void *PNew, int posAdd)
{
    // Отримуємо службовi елементи
    int cnt = ((int*)Start)[POS_CNT];
    int sz = ((int*)Start)[POS_SZ];

    // Якщо масив переповнений
    if (cnt == sz)
    {
        ExpandArrPtr(Start, sz, cnt);   // Розширення
    }
    if (posAdd == cnt)  // якщо додаємо у кiнець
    {
        Start[posAdd] = PNew;
    }
    else
    {   // Цикл зсуву елементiв вправо
        for (int j = cnt - 1; j >= posAdd; j--)
        {
            Start[j + 1] = Start[j];
        }
        Start[posAdd] = PNew;
    }
    cnt++;  // збiльшення к-стi елементiв
    ((int*)Start)[POS_CNT] = cnt;   // Оновлення службового елемента
}

/* Ф-цiя впорядкування масиву вказiвникiв пiсля замiни елемента.
   Отримує вказiвник на масив вказiвникiв певного рiвня, новий ключ, поточну позицiю
   ф-цiю порiвнянння. */
void zamInSort (void **&Start, QString key, int posCur, Comparator PFunc)
{
    int cnt = ((int*)Start)[POS_CNT];   // отримуємо к-сть елементiв у масивi

    // Цикл. Поки позицiя не дорiвнює нулю. Та поки елемент перед поточним бiльше поточного
    while ( (posCur != 0) && (PFunc (Start[posCur - 1], key)) == -1  )
    {
        // Обмiн мiсцями
        void *temp = Start[posCur];
        Start[posCur] = Start[posCur - 1];
        Start[posCur - 1] = temp;
        posCur--;       // перехiд на попереднiй iндекс
    }
    // Цикл. Поки позицiя не дорiвнює останньому iндексу. Та поки елемент пiсля поточного менше поточного
    while ( (posCur != (cnt - 1)) && (PFunc (Start[posCur + 1], key)) == 1 )
    {
        // Обмiн мiсцями
        void *temp = Start[posCur];
        Start[posCur] = Start[posCur + 1];
        Start[posCur + 1] = temp;
        posCur++;       // перехiд на наступний iндекс
    }
}

/* Ф-цiя виведення даних у QTreeWidget */
void ShowAll(void **Start, Ui::MainWindow *ui)
{
    ui->treeWidget_show->clear();                   // очистка та видалення
    QTreeWidgetItem *root = new QTreeWidgetItem();  // створення кореня
    QString nameR = ((TRoot*)Start[0])->name;
    QString numR = QString::number(((TRoot*)Start[0])->ses_count);

    root->setText(0, "Name of the show:  " + nameR + ", No. of series:  " + numR);
    ui->treeWidget_show->addTopLevelItem(root);     // додавання верхнього рiвня

    void **ContLvl = ((TRoot*)Start[0])->sublvl;    // пiдрiвень континентiв
    int cnt = ((int*)ContLvl)[POS_CNT];             // к-сть континентiв

    for (int i = 0; i < cnt; i++)       // цикл для континентiв
    {
        QTreeWidgetItem *childCont = new QTreeWidgetItem(); // дочiрнiй вузол
        QString nameCont = ((TCont*)ContLvl[i])->name;
        QString sqrCont = QString::number(((TCont*)ContLvl[i])->square);
        QString landmCont = QString::number(((TCont*)ContLvl[i])->landmass);
        childCont->setText(0, "Continent:  " + nameCont + ", Square:  " + sqrCont + " km^2, Landmass:  " + landmCont);
        root->addChild(childCont);  // додавання пiдрiвня континент для кореня

        void **CtrLvl = ((TCont*)ContLvl[i])->sublvl;   // пiдрiвень країн
        int cntCo = ((int*)CtrLvl)[POS_CNT];            // к-сть країн

        for (int j = 0; j < cntCo; j++)     // цикл виведення країн
        {
            QTreeWidgetItem *childCtr = new QTreeWidgetItem();  // дочiрнiй вузол
            QString nameCtr = ((TCountry*)CtrLvl[j])->name;
            QString capCtr = ((TCountry*)CtrLvl[j])->capital;
            QString numCtr = QString::number(((TCountry*)CtrLvl[j])->numOfEpisodes);
            childCtr->setText(0, "Country:  " + nameCtr + ", Capital:  " + capCtr + ", No. of episodes:  " + numCtr);

            childCont->addChild(childCtr);  // додавання дитини для континенту

            void **EpsLvl = ((TCountry*)CtrLvl[j])->sublvl; // пiдрiвень Епiзодiв
            int cntEps = ((int*)EpsLvl)[POS_CNT];           // к-сть епiзодiв

            for (int k = 0; k < cntEps; k++)        // цикл виведення епiзодiв
            {
                QTreeWidgetItem *childEps = new QTreeWidgetItem();
                QDate date = ((TEpisode*)EpsLvl[k])->date;
                QString dateEps = date.toString("dd/MM/yyyy");
                QString cityEps = ((TEpisode*)EpsLvl[k])->city;
                QString fio1 = ((TEpisode*)EpsLvl[k])->fio1;
                QString fio2 = ((TEpisode*)EpsLvl[k])->fio2;
                QString SpentSum = QString::number(((TEpisode*)EpsLvl[k])->SpentSum, 'f', 2);
                QString s_card1, s_card2;

                // обробка булевих змiнних про наявнiсть золотої карти
                bool card1 = ((TEpisode*)EpsLvl[k])->card1;
                if (card1)
                {
                    s_card1 = ", Golden Card: YES";
                }
                else
                {
                    s_card1 = ", Golden Card: NO";
                }

                bool card2 = ((TEpisode*)EpsLvl[k])->card2;
                if (card2)
                {
                    s_card2 = ", Golden Card: YES";
                }
                else
                {
                    s_card2 = ", Golden Card: NO";
                }

                // Формування тексту
                QString place_text = "First aired:  " + dateEps
                                   + "\nVisited City:  " + cityEps
                                   + "\nHost 1:  " + fio1 + s_card1
                                   + "\nHost 2:  " + fio2 + s_card2
                                   + "\nSpent Sum:  " + SpentSum + "\n";
                childEps->setText(0, place_text);
                childCtr->addChild(childEps);   // додавання дитини для країни
            }
        }
    }

    ui->treeWidget_show->expandAll();   // розгортання всього дерева
}
