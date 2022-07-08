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
    void fromSecond(double );
public:
    TimeSpan();
    TimeSpan(int hr, int mn, int s = 0);
    TimeSpan(double secs);
    QString toString(bool withSeconds = false);
    static bool Parse(const QString & , TimeSpan &);
    TimeSpan & operator = (TimeSpan  );
    friend bool operator > (TimeSpan , TimeSpan );
    friend bool operator < (TimeSpan one, TimeSpan two);
    friend bool operator <= (TimeSpan one, TimeSpan two);
    friend bool operator >= (TimeSpan one, TimeSpan two);
    friend bool operator == (TimeSpan one, TimeSpan two);
    friend bool operator == (TimeSpan one, double secs);
    friend TimeSpan operator -(TimeSpan one, TimeSpan two);
    friend TimeSpan operator +(TimeSpan one, TimeSpan two);
    TimeSpan & operator += (TimeSpan );
};
ostream& operator<< (ostream &stream, const TimeSpan& ts);
istream& operator>> (istream &is, TimeSpan & ts);
#endif // TIMESPAN_H
