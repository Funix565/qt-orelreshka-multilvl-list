#include <types_kurs.h>
#include <functions_kurs.h>
#include <types_minmax.h>
#include <float.h>
#include <deque>
#include <QDebug>

/* Ф-цiя пошуку половинним дiленням
   Отримує вказiвник на список, ключ для пошуку, iндекс знайденого елемента, результат пошуку, вказiвник на ф-цiю порiвняння*/
void FindElList(void **Start, QString key, int &Pos, bool &isFound, Comparator Pfunc)
{
    int mid;        // iндекс середнього елемента
    int hi, low;    // верхня та нижня межi
    isFound = false;// елемент не знайдено
    int resultComp;

    if (((int*)Start)[POS_CNT] == 0)
    {
        Pos = 0;    //якщо список пустий. Вставка у 0-iндекс
        return;
    }
    low = 0; // Нижня межа -- перший елемент
    hi = ((int*)Start)[POS_CNT] - 1; // Верхня -- останнiй елемент

    do
    {
        mid = (hi + low) / 2; // iндекс середнього елемента
        resultComp = Pfunc (Start[mid], key); // Виклик ф-цiї порiвняння
        if (resultComp == 0)    // Елемент знайдено
        {
            Pos = mid;  // Зберiгаємо позицiю елемента
            isFound = true;
            return;
        }
        if (resultComp == -1)
        {
            hi = mid - 1;   // Елемент знаходиться лiворуч вiд центру
        }
        else
        {
            low = mid + 1;  // Елемент знаходиться праворуч вiд центру
        }
    } while (low <= hi);
    Pos = low;  // Перебрано весь масив. Елемент не знайдено
                // Знайдена позицiя для вставки нового елемента.

}
/* Ф-цiя виведення даних про знайдений випуск */
void SearchEps(void **Start, Ui::MainWindow *ui, int posCont, int posCtr, int posEps)
{
    ui->plainTextEdit->clear(); // очищення

    // Данi про рiвень Континет
    QString nameCont = ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->name;
    QString sqrCont =  QString::number( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->square);

    // Данi про рiвень Країна
    QString nameCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->name;
    QString capCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->capital;

    // Данi про випуск
    QDate dateEps =  ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->date;
    QString date_str = dateEps.toString("dd/MM/yyyy");
    QString city = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->city;
    QString fio1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->fio1;
    QString fio2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->fio2;
    QString SpentSum = QString::number( ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->SpentSum, 'f', 2);
    QString s_card1, s_card2;

    bool card1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->card1;
    if (card1)
    {
        s_card1 = " Golden Card: YES";
    }
    else
    {
        s_card1 = " Golden Card: NO";
    }

    bool card2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[posEps])->card2;
    if (card2)
    {
        s_card2 = " Golden Card: YES";
    }
    else
    {
        s_card2 = " Golden Card: NO";
    }

    // Формування тексту для виведення
    QString place_Text = "Continent:  " + nameCont + ",  Square:  " + sqrCont + "\n"
                       + ((QString)("Country:  ")).rightJustified(12, ' ') + nameCtr + ",  Capital:  " + capCtr + "\n"
                       + ((QString)"First aired:  ").rightJustified(19, ' ') + date_str + "\n"
                       + ((QString)"Visited city:  ").rightJustified(19, ' ') + city + "\n"
                       + ((QString)"Host 1:  ").rightJustified(19, ' ') + fio1.leftJustified(17, ' ') + s_card1 + "\n"
                       + ((QString)"Host 2:  ").rightJustified(19, ' ') + fio2.leftJustified(17, ' ') + s_card2 + "\n"
                       + ((QString)"Spent Sum:  ").rightJustified(19, ' ') + SpentSum + "\n";
    ui->plainTextEdit->setPlainText(place_Text);    // розмiщення тексту
}

/* Ф-цiя виведення даних про знайдену країну*/
void SearchCtry(void **Start, Ui::MainWindow *ui, int posCont, int posCtr)
{
    ui->plainTextEdit->clear(); // очищення

    // Данi верхнього рiвня
    QString nameCont = ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->name;
    QString sqrCont =  QString::number( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->square);

    // Данi про країну
    QString nameCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->name;
    QString capCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->capital;
    QString epsCtr = QString::number(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->numOfEpisodes);

    // Формування тексту для виведення
    QString place_text = "Continent:  " + nameCont + ", Square:  " + sqrCont + "\n"
                       + ((QString)"Country:  ").rightJustified(14, ' ') + nameCtr + "\n" + ((QString)"Capital:  ").rightJustified(14, ' ') + capCtr + ", No. of Episodes:  " + epsCtr + "\n";

    // Отримуємо к-сть випускiв
    int cntEps = ((int*)(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl)) [POS_CNT];
    // Цикл виведення випускiв
    for (int k = 0; k < cntEps; k++)
    {
        QDate dateEps = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->date;
        QString date_str = dateEps.toString("dd/MM/yyyy");
        QString city = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->city;
        QString fio1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->fio1;
        QString fio2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->fio2;
        QString SpentSum = QString::number( ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->SpentSum, 'f', 2);
        QString s_card1, s_card2;

        bool card1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->card1;
        if (card1)
        {
            s_card1 = ", Golden Card: YES";
        }
        else
        {
            s_card1 = ", Golden Card: NO";
        }

        bool card2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl[posCtr])->sublvl[k])->card2;
        if (card2)
        {
            s_card2 = ", Golden Card: YES";
        }
        else
        {
            s_card2 = ", Golden Card: NO";
        }

        // Формування тексту про випуск
        QString appnd = ((QString)"First aired:  ").rightJustified(22, ' ') + date_str + "\n"
                + ((QString)"Visited city:  ").rightJustified(22, ' ') + city + "\n"
                + ((QString)"Host 1:  ").rightJustified(22, ' ') + fio1 + s_card1 + "\n"
                + ((QString)"Host 2:  ").rightJustified(22, ' ') + fio2 + s_card2 + "\n"
                + ((QString)"Spent Sum:  ").rightJustified(22, ' ') + SpentSum + "\n\n";
        place_text.append(appnd); // Доповнення
    }
    ui->plainTextEdit->setPlainText(place_text); // виведення тексту

}

/* Ф-цiя виведення даних про знайдений континент*/
void SearchCont(void **Start, Ui::MainWindow *ui, int posCont)
{
    ui->plainTextEdit->clear(); // очищення

    // Отримуємо данi про континент
    QString nameCont = ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->name;
    QString sqrCont =  QString::number( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->square);
    QString landmCont = QString::number( ((TCont*)((TRoot*)Start[0])->sublvl[posCont])->landmass, 'f', 2);
    int cntCtr = ((int*) (((TCont*)((TRoot*)Start[0])->sublvl[posCont])->sublvl)) [POS_CNT];
    QString cnt_s = QString::number(cntCtr); // К-сть доданих країн

    QString place_text = "Continent:  " + nameCont + "\nSquare:  " + sqrCont + "km^2, Landmass:  " + landmCont
                       + "\nCountries added:  " + cnt_s + "\n";

    ui->plainTextEdit->setPlainText(place_text);    // виведення
}

/* Ф-цiя пошуку випуску з максимальною та мiнiмальною сумою на континентi*/
void MinMaxSum(void **Start, Ui::MainWindow *ui)
{    
    ui->plainTextEdit->clear();     // початкове очищення поля виводу

    // Два деки для максимальних та мiнiмальних сум
    std::deque<TMinMax*> max_dq;
    std::deque<TMinMax*> min_dq;
    QString place_text = ""; // рядок результат

    int cntCont = ((int*)((TRoot*)Start[0])->sublvl) [POS_CNT]; // к-сть континентiв
    // цикл проходу по всiх континентах
    for (int i = 0; i < cntCont; i++)
    {
        // Початковi мiнiмальна та максимальна суми для континенту
        double MIN_S = DBL_MAX;
        double MAX_S = 0;

        QString nameCont = ((TCont*)((TRoot*)Start[0])->sublvl[i])->name.toUpper(); // Назва континнету великими буквами для кращого вiдображення
        QString nameCtr;

        int cntCtr = ((int*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl)[POS_CNT]; // к-сть країн на континентi
        // цикл проходу всiх країнах
        for (int j = 0; j < cntCtr; j++)
        {
            nameCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->name; // назва країни

            int cntEps = ((int*)(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl)) [POS_CNT]; // к-сть випускiв у країни
            // Цикл проходу по всiх випусках
            for (int k = 0; k < cntEps; k++)
            {
                QDate epsDate = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->date;
                double SpentSum = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->SpentSum;

                // Заповнення полiв вказiвника
                TMinMax* PNew = new TMinMax;
                PNew->Cont_name = nameCont;
                PNew->Ctr_name = nameCtr;
                PNew->dateEps = epsDate;
                PNew->SpentSum = SpentSum;

                if (SpentSum >= MAX_S) // Якщо поточна сума бiльше або дорiвнює максимальнiй
                {
                    MAX_S = SpentSum;         // нова максимальна сума
                    max_dq.push_front(PNew);  // додаємо елемент у початок деку
                }
                if (SpentSum <= MIN_S)  // Якщо поточна сума менше або дорiвнює мiнiмальнiй
                {
                    MIN_S = SpentSum;         // нова мiнiмальна сума
                    min_dq.push_back(PNew);   // додаємо елемент у кiнець деку
                }
            }
        }
        // Якщо епiзодiв не додано, деки порожнi
        if (max_dq.empty() && min_dq.empty())
        {
            continue; // перехiд на наступний континент
        }

        // Формування рядка результату
        place_text.append("Continent:  " + nameCont + ", Country:  " + nameCtr);

        // Цикл перегляду деку максимальних сум. Поки не порожнiй так поки сума елемента дорiвнює максимальнiй
        while ((max_dq.empty() == false) && max_dq.front()->SpentSum == MAX_S)
        {
            QString info = "\nDate:  " + max_dq.front()->dateEps.toString("dd/MM/yyyy")
                    + ", MAX  Spent Sum:  " + QString::number(max_dq.front()->SpentSum, 'f', 2);
            place_text.append(info);
            max_dq.pop_front(); // вилучення початкового елемента
        }

        // Цикл перегляду мiнiмальних сум. Поки не порожнiй так поки сума елемента дорiвнює мiнiмальнiй
        while ((min_dq.empty() == false) && min_dq.back()->SpentSum == MIN_S)
        {
            QString info = "\nDate:  " + min_dq.back()->dateEps.toString("dd/MM/yyyy")
                    + ", MIN  Spent Sum:  " + QString::number(min_dq.back()->SpentSum, 'f', 2);
            place_text.append(info);
            min_dq.pop_back();  // вилучення останнього елемента
        }
        place_text.append("\n\n");

        // Очищення декiв
        min_dq.clear();
        max_dq.clear();
    }

    // Якщо континентiв не iснує, результат не сформовано
    if (place_text.length() == 0)
    {
        // Повiдомлення
        ui->plainTextEdit->setPlaceholderText("Nothing was found");
        return;
    }

    ui->plainTextEdit->setPlainText(place_text); // Виведення результату
}

/* Ф-цiя складання рейтингу за кiлькiстю випускiв. */
void CtrRate(void **Start, Ui::MainWindow *ui)
{
    ui->plainTextEdit->clear();
    QString place_text = "";

    // Iнiцiалiзацiя
    TMinMax *StartRate;
    initMinMax(StartRate);

    int cntCont = ((int*)((TRoot*)Start[0])->sublvl) [POS_CNT]; // к-сть континентiв
    // цикл проходу по всiх континентах
    for (int i = 0; i < cntCont; i++)
    {
        int cntCtr = ((int*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl)[POS_CNT]; // к-сть країн на континентi
        // цикл проходу всiх країнах
        for (int j = 0; j < cntCtr; j++)
        {
            QString nameCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->name; // назва країни
            int epsNum = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->numOfEpisodes; // к-сть випускiв

            // Елемент лiнiйного списку
            TMinMax* PNew = new TMinMax;
            PNew->Ctr_name = nameCtr;   // iм'я країни
            PNew->EpsNum = epsNum;      // К-сть епiзодiв
            PNew->next = NULL;

            addToSortRateList(StartRate, PNew);  // додавання в упорядковий список
        }
    }
    // Якщо жодного елемента не додали
    if (StartRate == NULL)
    {
        ui->plainTextEdit->setPlaceholderText("There is no Continents or Countries.");
        return;
    }

    // Початок виведеня iнформацiї
    place_text.append("Country \t\t No. of Episodes");
    TMinMax *wp;        // допомiжний вказiвник
    wp = StartRate;     // починаємо з першого
    while (wp != NULL)
    {
        QString ctr = wp->Ctr_name;                    // Назва країни
        QString epsN = QString::number(wp->EpsNum);    // К-сть випускiв

        // Формуємо рядок з вирiвнюванням по лiвому краю
        QString s_format = "\n" + ctr.leftJustified(28, ' ') + epsN;
        place_text.append(s_format);
        wp = wp->next; // переходимо на наступний елемент списку
    }

    ui->plainTextEdit->setPlainText(place_text);  // виведення

    delList(StartRate);   // видалення списку
}

/* Ф-цiя пошуку усiх випускiв, де вказаний ведучий має золоту карту*/
void FindHost(void **Start, Ui::MainWindow *ui, QString keyHost)
{
    ui->plainTextEdit->clear();
    QString place_text = "";

    // iнiцiалiзацiя лiнiйного списку
    TMinMax *StartHost;
    initMinMax(StartHost);

    int cntCont = ((int*)((TRoot*)Start[0])->sublvl) [POS_CNT]; // к-сть континентiв
    // цикл проходу по всiх континентах
    for (int i = 0; i < cntCont; i++)
    {
        int cntCtr = ((int*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl)[POS_CNT]; // к-сть країн на континентi
        // цикл проходу всiх країнах
        for (int j = 0; j < cntCtr; j++)
        {
            QString nameCtr = ((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->name; // назва країни
            int cntEps = ((int*)(((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl)) [POS_CNT]; // к-сть випускiв у країни
            // Цикл проходу по всiх випусках
            for (int k = 0; k < cntEps; k++)
            {
                // Отримуємо данi для структури.
                // Дату, Країну, Ведучого, Мiсто, Наявнiсть карти
                QDate epsDate = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->date;
                QString mb_fio1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->fio1;
                bool card1 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->card1;
                QString mb_fio2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->fio2;
                bool card2 = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->card2;
                QString vis_city = ((TEpisode*)((TCountry*)((TCont*)((TRoot*)Start[0])->sublvl[i])->sublvl[j])->sublvl[k])->city;

                // Якщо перший ведучий мультисписку збiгається з ключем i має золоту карту
                if (mb_fio1 == keyHost && card1 == true)
                {
                    // Створюємо елемент для лiнiйного списку
                    TMinMax *PNew = new TMinMax;
                    PNew->Ctr_name = nameCtr;
                    PNew->dateEps = epsDate;
                    PNew->fio = mb_fio1;
                    PNew->City_name = vis_city;
                    PNew->card = card1;
                    PNew->next = NULL;
                    addToStartHost(StartHost, PNew);    // Додаємо на початок
                }
                else if (mb_fio2 == keyHost && card2 == true) // Якщо другий ведучий мультисписку збiгається з ключем i має золоту карту
                {
                    // Створюємо елемент для лiнiйного списку
                    TMinMax *PNew = new TMinMax;
                    PNew->Ctr_name = nameCtr;
                    PNew->dateEps = epsDate;
                    PNew->fio = mb_fio2;
                    PNew->City_name = vis_city;
                    PNew->card = card2;
                    PNew->next = NULL;
                    addToStartHost(StartHost, PNew);   // Додаємо на початок
                }
            }
        }
    }
    // Якщо немає випускiв або континентiв - у список нiчого не додано, бо нiчого не знайдено
    if (StartHost == NULL)
    {
        ui->plainTextEdit->setPlaceholderText("Nothing was found. Try another Host");
        return;
    }

    // Початок виведення iнформацiї.
    TMinMax *wp;        // Допомiжний вказiвник
    wp = StartHost;     // Починаємо з початку
    // поки не продивимося усi елементи
    while (wp != NULL)
    {
        QString date = wp->dateEps.toString("dd/MM/yyyy");
        QString ctr = wp->Ctr_name;
        QString host = wp->fio;
        QString city = wp->City_name;
        QString card = QVariant(wp->card).toString();

        QString s_format = date.leftJustified(20, ' ') + ctr + "\n"
                         + host.leftJustified(20, ' ') + city.leftJustified(20, ' ') + card +"\n\n";
        place_text.append(s_format);
        wp = wp->next;  // перехiд на наступний елемент
    }

    ui->plainTextEdit->setPlainText(place_text);    // виведення
}
