#ifndef TIMESPAN_H
#define TIMESPAN_H
#include <string>
#include <iostream>
#include <QString>

using namespace std;

class TimeSpan
{
    int m_hours; // часы
    int m_minutes; // минуты
    int m_seconds; // секунды
    double Seconds();
public:
    TimeSpan();
    TimeSpan(int hr, int mn, int s);
    TimeSpan(double secs);
    QString toString();
    static bool Parse(const QString & , TimeSpan &);
    TimeSpan & operator = (TimeSpan  );
    friend bool operator > (TimeSpan , TimeSpan );
    friend bool operator < (TimeSpan one, TimeSpan two);
    friend bool operator <= (TimeSpan one, TimeSpan two);
    friend bool operator >= (TimeSpan one, TimeSpan two);
    friend bool operator == (TimeSpan one, TimeSpan two);
    friend TimeSpan operator -(TimeSpan one, TimeSpan two);
    friend TimeSpan operator +(TimeSpan one, TimeSpan two);


};
ostream& operator<< (ostream &stream, const TimeSpan& ts);
istream& operator>> (istream &is, TimeSpan & ts);
#endif // TIMESPAN_H
