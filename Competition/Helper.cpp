#include<iostream>
#include <cmath>
#include<vector>
#include <algorithm>
#include <Helper.h>

using namespace std;

// преобразование строки strTime в дату и время tinp
// возврат true - преобразование успешно,
// false - были ошибки преобразования
bool ParseTime(string strTime, time_t &tinp)
{
    time_t t;
    char tpart[4];
    time(&t);
    tm *time =  localtime(&t);
    if(strTime[2] != ':' && strTime[5] != ':')
        return false;

    // преобразовать время в  чася минуты и секунды
    tpart[0] = strTime[0];
    tpart[1] = strTime[1];
    tpart[3] = '\0';
    if(!isdigit(tpart[0]) || !isdigit(tpart[1]))
        return false;
    int i = atoi(tpart);
    if(i < 0 || i > 23 )
        return false;
    time->tm_hour = i;

    tpart[0] = strTime[3];
    tpart[1] = strTime[4];
    tpart[3] = '\0';
    if(!isdigit(tpart[0]) || !isdigit(tpart[1]))
        return false;
    i = atoi(tpart);
    if( i < 0 || i > 59 )
        return false;
    time->tm_min = i;

    tpart[0] = strTime[6];
    tpart[1] = strTime[7];
    tpart[3] = '\0';
    if(!isdigit(tpart[0]) || !isdigit(tpart[1]))
        return false;
    i = atoi(tpart);
    if (i < 0 || i > 59 )
        return false;
    time->tm_sec = i;

    time->tm_mon = 1;

    tinp = mktime(time);

    return true;

}

// Строковое представление разницы во времени
// end - время завершения
// begin - время начала
// строковое представление времени
string StringTimeDiff(time_t end, time_t begin)
{
    double secs = difftime(end, begin);
    int hours = (int)floor(secs/3600.0);
    int mins = (int)floor((secs - hours*3600.0)/60.0);
    int seconds = (int)floor(secs - hours*3600.0 - mins*60.0);
    char buff[16];
    sprintf(buff,"%02d:%02d:%02d", hours, mins, seconds);
    string ret = buff;
    return ret;
}

// Ввод данных участников соревнований по возрастным группам
// youngGroup - группа от 18 до 30 лет
// middleGroup  - группа от 31 до 55 лет
// oldGroup - группа от 56 до 75 лет
void InputDataFromConsole(vector<Participant> &youngGroup,
                          vector <Participant> &middleGroup,
                          vector<Participant> &oldGroup)
{
    cout << "Введите участников соревнований" << endl;

    time_t tbegin[3];
    string age_groups[3] = {"от 18 до 30", "от 31 до 55", "от 56 до 75"};

    for(int i =0 ; i < 3; i++)
    {
          while(true)
          {
              string begin_time;
              cout << "Введите время старта для возрастной \r\nгруппы " << age_groups[i] <<" лет в формате HH:mm:ss: ";
              cin >> begin_time;
              if(ParseTime(begin_time, tbegin[i])) break;
                cerr << "Ошибка задания времени: " << begin_time[i] << endl;
          }
    }

    int count;
    while(true)
    {
        cout << "Введите количество участников соревнований (от 1 до 18): ";
        cin >> count;
        if(count >= 1 && count <= 18) break;
        else
          cerr << "Число участников соревнований должно находится в пределах от 1 до 18" << endl;
    }

    for(int i =0 ; i < count; i++)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string fio;
        cout << "Введите ФИО участника: ";
        getline(cin,fio );

        int age = 0;
        do {
            cout << "Введите возраст участника: ";
            cin >> age;
          if(age < 18 || age > 75)
            cerr << "Возраст должен находиться в пределах от 18 до 75 лет." << endl;
        } while(age < 18 || age > 75);

        time_t btime;
        if(age >=18 && age <=30)
            btime = tbegin[0];
        else if (age >=31 && age <=55)
            btime = tbegin[1];
        else
            btime = tbegin[2];

        string s1;
        time_t etime;
        while(true)
        {
            cout << "Введите время финиша участника в формате HH:mm:ss:";
            cin >> s1;
            if(ParseTime(s1, etime))
            {
                if(etime <= btime)
                    cerr << "Ошибка: старт не может быть раньше финиша" << endl;
                else
                    break;
            }
            else
                cerr << "Ошибка задания времени: " << s1 << endl;
        }

        Participant person;
        person.Name = fio;
        person.Age = age;
        person.TimeBegin = btime;
        person.TimeEnd = etime;
        if(age >=18 && age <=30)
        {
            if(youngGroup.size() >= 18)
                cerr << "Число участников не может быть более 18" << endl;
            else
                youngGroup.push_back(person);
        }
        else if (age >=31 && age <=55)
        {
            if(middleGroup.size() >= 18)
                cerr << "Число участников не может быть более 18" << endl;
            else
                middleGroup.push_back(person);
        }
        else
        {
            if(oldGroup.size() >= 18)
                cerr << "Число участников не может быть более 18" << endl;
            else
                oldGroup.push_back(person);
        }

    }
}

// Отображение time_t в виде строки
string Timet2String(time_t t)
{
    tm *tm_time = localtime(&t);
    char buff[16];
    sprintf(buff,"%02d:%02d:%02d", tm_time->tm_hour, tm_time->tm_min, tm_time->tm_sec);
    string ret = buff;
    return ret;
}

// Ввод данных об участниках соревнований по возрастной группе
// group - возрастная группа
// count - число участников
void OutputParticipantsToConsole(vector<Participant> &group, int count = 0)
{
    int n;
    if(count == 0) n = group.size();
    else n = min(count, (int)group.size());
    for(int i = 0; i < n; i++)
    {
        Participant elem  = group[i];
        cout << "-------------------------------------------------------" << endl;
        cout << "ФИО: " << elem.Name << endl;
        cout << "Возраст: " << elem.Age << endl;
        cout << "Время старта: " << Timet2String(elem.TimeBegin) << endl;
        cout << "Время финиша: " << Timet2String(elem.TimeEnd) << endl;
        cout << "Время прохождения дистанции: " << StringTimeDiff(elem.TimeEnd, elem.TimeBegin) << endl;
    }
}
// Функуия сравнения по времени для сортировки списка
int CompareByFinish(Participant one, Participant two)
{
      if(one.TimeEnd > two.TimeEnd) return 1;
      else if(one.TimeEnd < two.TimeEnd) return -1;
      else return 0;
}


