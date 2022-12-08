#include "abee.h"
#include <cmath>

ABee::~ABee() {}

ABee::ABee(double live_limit, double consumption, int time_limit)
{
    m_liveTimeLimit = time_limit;
    m_consumption = m_normConsumption = consumption;
    m_liveEnergyLimit = m_currentLiveEnergy = live_limit;
}

double ABee::liveEnergyLimit() {return m_liveEnergyLimit;}
int ABee::liveTimeLimit() {return m_liveTimeLimit;}
double ABee::normConsumption() {return m_normConsumption;}

double ABee::consumption()
{
    return m_consumption;
}
void ABee::setConsumption(double val)
{
    // потебление не может быть отрицательным или превышать норматив
    if(val < 0.0)
        m_consumption = 0.0;
    else if (val == 0.0)
        m_consumption = m_normConsumption;
    else
        m_consumption = fmin(val,m_normConsumption);

     // увеличение или уменьшение жизненной силы в зависимости от
     // норматива потребления
     double k = m_consumption/m_normConsumption;
     if(k > 1.0) k = 1.0 + (m_normConsumption - m_consumption)/m_normConsumption;
     m_currentLiveEnergy *= k;
     m_currentLiveEnergy = fmin(m_currentLiveEnergy, m_liveEnergyLimit);
}

void ABee::decreaseLiveEnergy()
{
    m_currentLiveEnergy -= m_consumption;
    if(m_currentLiveEnergy < 0.0) m_currentLiveEnergy = 0.0;
}

void ABee::increaseLiveEnergy()
{
    m_currentLiveEnergy += m_consumption;
    if(m_currentLiveEnergy > m_liveEnergyLimit) m_liveEnergyLimit = m_currentLiveEnergy;
}
double ABee::currentLiveEnergy() { return m_currentLiveEnergy;}


MotherBee::MotherBee(double live_limit, double consumption,int time_limit) :
    ABee(live_limit, consumption, time_limit)
{

}

MotherBee * MotherBee::clone()
{
    MotherBee *bee = new MotherBee(m_liveEnergyLimit, m_consumption, m_liveTimeLimit);
    return bee;
}

CollectorBee::CollectorBee(double live_limit, double consumption, double time_limit, double nectarToTrip) :
    ABee(live_limit, consumption, time_limit)
{
    m_Status = IDLE;
    m_idleConsumption = consumption;
    m_nectarToTrip = nectarToTrip;

}

CollectorStatus CollectorBee::status() {return m_Status;}
void CollectorBee::setStatus(CollectorStatus stat, double builderConsumption)
{
    m_Status = stat;
    if(m_Status == IN_TRIP)
        m_normConsumption = m_idleConsumption + m_nectarToTrip*0.2;
    else if(m_Status == RECREATION)
          m_normConsumption = 4.0*builderConsumption;
    else
        m_normConsumption = m_idleConsumption;

}

double CollectorBee::nectarToTrip()
{
    return m_nectarToTrip;
}

BuilderBee::BuilderBee(double live_limit, double consumption, int time_limit) :
    ABee(live_limit, consumption, time_limit)
{

}

double BuilderBee::produceHoney(double qnectar)
{
    // строитель за один ход перерабатывает не более 10 ед. нектара
    // переработка исходя из 7 ед. нектара на 10 ед. мёда
    return fmin(qnectar,10.0)*0.7;

}
