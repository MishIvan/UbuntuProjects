#include "abee.h"

ABee::~ABee() {}

ABee::ABee(double live_limit, double consumption, int time_limit)
{
    m_liveTimeLimit = time_limit;
    m_consumption = consumption;
    m_liveEnergyLimit = m_currentLiveEnergy = live_limit;
}

double ABee::liveEnergyLimit() {return m_liveEnergyLimit;}
void ABee::setLiveEnergyLimit(double val)
{
    m_liveEnergyLimit = val < 0.0 ? 0.0 : val;

}

int ABee::liveTimeLimit() {return m_liveTimeLimit;}
void ABee::setLiveTimeLimit(int val) {m_liveTimeLimit = val < 0.0 ? 0.0 : val;}

double ABee::consumption() {return m_consumption;}
void ABee::setConsumption(double val) {m_consumption = val < 0.0 ? 0.0 : val;}

void ABee::increaseLiveEnergy()
{
    m_currentLiveEnergy -= m_consumption;
    if(m_currentLiveEnergy < 0.0) m_currentLiveEnergy = 0.0;
}
void ABee::decreaseLiveEnergy()
{
    m_currentLiveEnergy += m_consumption;
    if(m_currentLiveEnergy > m_liveEnergyLimit) m_liveEnergyLimit = m_currentLiveEnergy;
}
double ABee::currentLiveEnergy() { return m_currentLiveEnergy;}


MotherBee::MotherBee(double live_limit, double consumption,int time_limit) :
    ABee(live_limit, consumption, time_limit)
{

}
CollectorBee::CollectorBee(double live_limit, double consumption, double time_limit, double nectarToTrip) :
    ABee(live_limit, consumption, time_limit)
{
    m_Status = IDLE;
    m_idleConsumption = consumption;
    m_nectarToTrip = nectarToTrip;

}
CollectorStatus CollectorBee::status() {return m_Status;}
void CollectorBee::setStatus(CollectorStatus stat)
{
    m_Status = stat;
}
void CollectorBee::setConsumption(double builderConsumption)
{
    if(m_Status == IN_TRIP)
        m_consumption = m_idleConsumption + m_nectarToTrip*0.2;
    else if(m_Status == RECREATION)
          m_consumption = 4.0*builderConsumption;
    else
        m_consumption = m_idleConsumption;

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
    if(qnectar > 10.0) qnectar = 10.0; // строитель за один ход перерабатывает не более 10 ед. нектара
    return qnectar*0.7; // переработка исходя из 7 ед. нектара на 10 ед. мёда

}
