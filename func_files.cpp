#include <types_kurs.h>
#include <functions_kurs.h>
#include <QFile>
#include <QVariant>
#include <QStringList>
#include <QDebug>

/* Ф-цiя збереження даних у файл */
void saveINFile (void **Start, QString path)
{
    QFile file(path);   // файлова змiнна для заданого шляху
    // Створення шаблонiв. <Вiдсоток><Число> буде замiнено на переданий текст
    QString pattern  = "\"%1\":%2,%3\n";                                    // шаблон для запису Кореня
    QString pattern1 = "\"%1\":%2,%3,%4\n";                                 // шаблон для запису Континенту та Країни
    QString pattern2 = "\"%1\":%2,%3,%4,%5,%6,%7,%8\n";                     // шаблон для запису Випуску
    QString tagRoot = "R", tagCont = "C", tagCtr = "T", tagEps = "E";       // Мiтки для вiдповiдних рiвнiв

    file.open(QIODevice::WriteOnly | QIODevice::Text); // вiдкриття файлу для запису тексту

    // Доступ до кореня
    TRoot *r = ((TRoot*)Start[0]);
    // запис у файл вiдповiдних полiв: мiтка, iм'я програми, к-сть сезонiв
    file.write(pattern.arg(tagRoot, r->name, QString::number(r->ses_count)).toUtf8());

    int cntCont = ((int*)((TRoot*)Start[0])->sublvl) [POS_CNT]; // к-сть континентiв
    for (int j = 0; j < cntCont; j++)   // цикл перегляду континентiв
    {
        TCont *cont = ((TCont*)r->sublvl[j]); // Доступ до Континенту
        // Запис: Мiтка, назва, площа, вiдсоток сушi
        file.write(pattern1.arg(tagCont, cont->name, QString::number(cont->square), QString::number(cont->landmass, 'f', 2)).toUtf8());

        // Якщо пiдрiвень не заповнений i країн немає
        if (cont->sublvl == NULL)
        {
            continue;   // наступний континент
        }

        int cntCtr = ((int*)((TCont*)((TRoot*)Start[0])->sublvl[j])->sublvl)[POS_CNT]; // к-сть країн на континентi
        // Цикл перегляду країн
        for (int k = 0; k < cntCtr; k++)
        {
            TCountry *ctr = ((TCountry*)cont->sublvl[k]);  // Доступ до Країни
            // Запис: мiтка, назва мiста, столиця, к-сть випускiв
            file.write(pattern1.arg(tagCtr, ctr->name, ctr->capital, QString::number(ctr->numOfEpisodes)).toUtf8());
            // Якщо пiдрiвень не заповнений i випускiв немає
            if (ctr->sublvl == NULL)
            {
                continue;   // наступна країна
            }

            int cntEps = ((int*)(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[j])->sublvl[k])->sublvl))[POS_CNT]; // к-сть випускiв у країни
            for (int m = 0; m < cntEps; m++) // цикл перегляду Випускiв
            {
                TEpisode* eps = ((TEpisode*)ctr->sublvl[m]);   // Доступ до Випуску
                // Запис: мiтка, дата, мiсто, ведучий1, карта1, ведучий2, карта2, сума
                file.write(pattern2.arg(tagEps, eps->date.toString("dd/MM/yyyy"), eps->city, eps->fio1, QVariant(eps->card1).toString(), eps->fio2, QVariant(eps->card2).toString(), QString::number(eps->SpentSum, 'f', 2  )).toUtf8());
            }
        }
    }
    file.close();   // закриваємо файл
}

/* Ф-цiя заповнення мультисписку з файлу */
void openFROMFile (void **&Start, QString path)
{
    void **nStart = InitArr();  // Допомiжний вказiвник
    QFile file (path);          // файлова змiнна
    QString tagRoot = "\"R\":", tagCont = "\"C\":", tagCtr = "\"T\":", tagEps = "\"E\":";       // Мiтки, як вони записанi у файлi
    int rCnt = 0, cCnt = 0, tCnt = 0, eCnt = 0;  // позицiї додавання елементiв у список

    file.open(QIODevice::ReadOnly | QIODevice::Text); // вiдкриття файлу для читання тексту

    // Цикл, поки не досягнемо кiнця файлу
    while (!file.atEnd())
    {
        QByteArray bytes = file.readLine(); // читаємо рядок файлу, доки не зустрiнемо "\n"
        QString line(bytes); // рядок з файлу

        line.remove("\n");  // видалення ознаки кiнця рядка
        QStringList item = line.split(","); // розбиваємо рядок на масив рядкiв з даними про рiвень

        // Якщо мiтка - Корiнь
        if (line.startsWith(tagRoot))
        {
            item[0] = item[0].remove(tagRoot); // видаляємо мiтку

            // Створюмо вказiвник для списку
            TRoot *rt = new TRoot;
            // Заносимо данi у список, звертаючись до них через iндекси так, як поля були записанi у файл
            rt->name = item[0];
            rt->ses_count = item[1].toInt();
            rt->sublvl = InitArr();

            addToSort(nStart, (void*)rt, rCnt++); // додавання. Змiщення позицiї для наступного елемента
            cCnt = tCnt = eCnt = 0; // Позицiї для пiдрiвнiв - нульовi
        }
        else if (line.startsWith(tagCont)) // Якщо мiтка - Континент
        {
            bool ok;
            item[0] = item[0].remove(tagCont);   // видаляємо мiтку

            TCont *c = new TCont; // вказiвник
            // Звертаємося до даних у тому порядку, як вон були записанi у файл
            c->name = item[0];
            c->square = item[1].toUInt(&ok, 10);
            c->landmass = item[2].toDouble();
            c->sublvl = InitArr();

            // iнiцiалiзацiя пiдрiвня, якщо потрiбно
            if (((TRoot*)nStart[rCnt - 1])->sublvl == nullptr)
            {
                ((TRoot*)nStart[rCnt - 1])->sublvl = InitArr();
            }

            addToSort(((TRoot*)nStart[rCnt - 1])->sublvl, (void*)c, cCnt++); // додавання. Змiщення позицiї
            tCnt = eCnt = 0; // Позицiї елементiв пiдрiвнiв - нульовi
        }
        else if (line.startsWith(tagCtr))   // Якщо мiтка - Країна
        {
            item[0] = item[0].remove(tagCtr);   // видаляємо мiтку

            TCountry* ctr = new TCountry;
            ctr->name = item[0];
            ctr->capital = item[1];
            ctr->numOfEpisodes = item[2].toInt();
            ctr->sublvl = InitArr();

            // Якщо потрiбно - iнiцiалiзацiя пiдрiвня
            if (((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl == nullptr)
            {
                ((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl = InitArr();
            }
            addToSort(((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl, (void*)ctr, tCnt++ );
            eCnt = 0; // Позицiя елемента пiдрiвня - нуль
        }
        else if (line.startsWith(tagEps))   // Якщо мiтка - Випуск
        {
            item[0] = item[0].remove(tagEps);   // видаляємо мiтку

            TEpisode *e = new TEpisode;            

            e->date = QDate::fromString(item[0], "dd/MM/yyyy");
            e->city = item[1];
            e->fio1 = item[2];
            e->card1 = QVariant(item[3]).toBool();
            e->fio2 = item[4];
            e->card2 = QVariant(item[5]).toBool();
            e->SpentSum = item[6].toDouble();

            if (((TCountry*)((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl[tCnt - 1])->sublvl == nullptr)
            {
                ((TCountry*)((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl[tCnt - 1])->sublvl = InitArr();
            }

            addToSort( ((TCountry*)((TCont*)((TRoot*)nStart[rCnt - 1])->sublvl[cCnt - 1])->sublvl[tCnt - 1])->sublvl, (void*)e, eCnt++ );
        }
    }
    file.close();   // закриваємо файл
    Start = nStart; // Старт основного мультисписку тепер вказує на створений список из файлу
}

















