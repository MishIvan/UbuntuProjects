#include "timespan.h"
#include <math.h>
#include <regex>

TimeSpan::TimeSpan()
{
        m_hours = 0; m_minutes = 0; m_seconds = 0;
}
TimeSpan::TimeSpan(int hr, int mn, int s)
{
    m_hours = hr; m_minutes = mn; m_seconds = s;
}
TimeSpan::TimeSpan(double secs)
{
    fromSecond(secs);
}

TimeSpan::TimeSpan(QTime tm, bool roundToMinute)
{
    m_hours = tm.hour();
    m_minutes = tm.minute();
    m_seconds = tm.second();

    if(roundToMinute)
    {
        if(m_seconds >= 30)
        {
            m_seconds = 0;
            m_minutes++;
        }
        if(m_minutes > 59)
        {
            m_hours++;
            m_minutes = 0;
        }
    }
}

double TimeSpan::Seconds()
{
    return m_hours*3600.0 + m_minutes*60.0 + m_seconds;
}

void TimeSpan::fromSecond(double secs)
{
    m_hours = (int)floor(secs/3600.0);
    m_minutes = (int)floor((secs - m_hours*3600.0)/60.0);
    m_seconds = (int)floor(secs - m_hours*3600.0 - m_minutes*60.0);

}

QString TimeSpan::toString(bool withSeconds)
{
    char buff[16];
    if(withSeconds)
        sprintf(buff,"%02d:%02d:%02d", m_hours, m_minutes, m_seconds);
    else
        sprintf(buff,"%02d:%02d", m_hours, m_minutes);
    return QString(buff);

}

bool TimeSpan::Parse(const QString &strTime, TimeSpan &ts)
{
     regex rexp ("[0-9]+");
     string s1 = strTime.toStdString();
     smatch m;
     int val[3] = {0,0,0};
     int j = 0;
     while(regex_search(s1, m, rexp))
     {
         int n = m.size();
         for(int i =0; i < n; i++)
         {
             try {
                 val[j] = stoi(m[i]);
             } catch (...) {
                 return false;
             }
         }
         j++;
         s1 = m.suffix().str();
     }
     if(val[0] < 0 || val[0] > 23 || val[1] < 0 || val[1] > 59 || val[2] < 0 || val[2] > 59)
         return false;
     ts = TimeSpan(val[0], val[1], val[2]);
        return true;

}

// перегрузка операторов
TimeSpan& TimeSpan::operator = (TimeSpan ts)
{
    this->m_hours = ts.m_hours;
    this->m_minutes = ts.m_minutes;
    this->m_seconds = ts.m_seconds;
    return *this;
}

TimeSpan & TimeSpan::operator += (TimeSpan ts)
{
    double secs = this->Seconds();
    secs += ts.Seconds();
    this->fromSecond(secs);
    return *this;
}

bool operator > (TimeSpan one, TimeSpan two)
{
      return one.Seconds() > two.Seconds();
}

bool operator >= (TimeSpan one, TimeSpan two)
{
      return one.Seconds() >= two.Seconds();
}


bool operator < (TimeSpan one, TimeSpan two)
{
      return one.Seconds() < two.Seconds();
}

bool operator <= (TimeSpan one, TimeSpan two)
{
      return one.Seconds() < two.Seconds();
}


bool operator == (TimeSpan one, TimeSpan two)
{
      return one.Seconds() == two.Seconds();
}

bool operator == (TimeSpan one, double secs)
{
    return one.Seconds() == secs;
}

 TimeSpan operator - (TimeSpan one, TimeSpan two)
{
      double secs =  one.Seconds() - two.Seconds();
      return TimeSpan(secs);
}

 TimeSpan operator + (TimeSpan one, TimeSpan two)
{
      double secs =  one.Seconds() + two.Seconds();
      return TimeSpan(secs);
}

 ostream& operator<< (ostream &os, const  TimeSpan & ts)
 {
      os << ((TimeSpan)ts).toString().toStdString();
      return os;
 }

 istream& operator>> (istream &is,  TimeSpan & ts)
 {
      string s1;
      is >> s1;
      TimeSpan::Parse(QString(s1.data()), ts);
      return is;
 }
