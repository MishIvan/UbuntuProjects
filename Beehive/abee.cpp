#include "abee.h"
#include <cmath>

ABee::~ABee() {}

double ABee::consumption() {  return m_consumption; }
double ABee::currentLiveEnergy() { return m_currentLiveEnergy;}


void ABee::decreaseLiveEnergy()
{
    m_currentLiveEnergy -= m_consumption;
    if(m_currentLiveEnergy < 0.0) m_currentLiveEnergy = 0.0;
}

int ABee::age() { return m_age;}



MotherBee::MotherBee()
{
    m_currentLiveEnergy = MotherBee::m_liveEnergyLimit;
    m_age = 0;
    m_consumption = MotherBee::m_normConsumption;
}

void MotherBee::setConsumption(double val)
{
    // потебление не может быть отрицательным или превышать норматив
    if(val < 0.0)
        m_consumption = 0.0;
    else if (val == 0.0)
        m_consumption = MotherBee::m_normConsumption;
    else
        m_consumption = fmin(val,MotherBee::m_normConsumption);

     // увеличение или уменьшение жизненной силы в зависимости от
     // норматива потребления
     double k = m_consumption/MotherBee::m_normConsumption;
     if(k > 1.0) k = 1.0 + (MotherBee::m_normConsumption - m_consumption)/MotherBee::m_normConsumption;
     m_currentLiveEnergy *= k;
     m_currentLiveEnergy = fmin(m_currentLiveEnergy, MotherBee::m_liveEnergyLimit);
}


void MotherBee::increaseLiveEnergy()
{
    m_currentLiveEnergy += m_consumption;
    if(m_currentLiveEnergy > MotherBee::m_liveEnergyLimit)
        m_currentLiveEnergy = MotherBee::m_liveEnergyLimit;
}

void MotherBee::setAge(int age)
{
    if(age > MotherBee::m_ageLimit)
        m_age = MotherBee::m_ageLimit;
    else
        m_age = age;
}


CollectorBee::CollectorBee()
{
    m_Status = IDLE;
    m_normConsumption = CollectorBee::m_idleConsumption;
    m_consumption = CollectorBee::m_idleConsumption;

    m_currentLiveEnergy = CollectorBee::m_liveEnergyLimit;
    m_age = 0;

}

CollectorStatus CollectorBee::status() {return m_Status;}
void CollectorBee::setStatus(CollectorStatus stat)
{
    m_Status = stat;
    if(m_Status == IN_TRIP)
        m_normConsumption = CollectorBee::m_idleConsumption + CollectorBee::m_nectarToTrip*0.2;
    else if(m_Status == RECREATION)
          m_normConsumption = 4.0*BuilderBee::m_normConsumption;
    else
        m_normConsumption = CollectorBee::m_idleConsumption;

}
void CollectorBee::setConsumption(double val)
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
     m_currentLiveEnergy = fmin(m_currentLiveEnergy, CollectorBee::m_liveEnergyLimit);
}

void CollectorBee::increaseLiveEnergy()
{
    m_currentLiveEnergy += m_consumption;
    if(m_currentLiveEnergy > CollectorBee::m_liveEnergyLimit)
        m_currentLiveEnergy = CollectorBee::m_liveEnergyLimit;
}

void CollectorBee::setAge(int age)
{
    if(age > CollectorBee::m_ageLimit)
        m_age = CollectorBee::m_ageLimit;
    else
        m_age = age;
}

double CollectorBee::normConsumption()
{
    return m_normConsumption;
}

BuilderBee::BuilderBee() : ABee()
{
    m_currentLiveEnergy = BuilderBee::m_liveEnergyLimit;
    m_age = 0;
    m_consumption = BuilderBee::m_normConsumption;

}

double BuilderBee::produceHoney(double qnectar)
{
    // строитель за один ход перерабатывает не более 10 ед. нектара
    // переработка исходя из 7 ед. нектара на 10 ед. мёда
    return fmin(qnectar,10.0)*0.7;

}
void BuilderBee::setConsumption(double val)
{
    // потебление не может быть отрицательным или превышать норматив
    if(val < 0.0)
        m_consumption = 0.0;
    else if (val == 0.0)
        m_consumption = BuilderBee::m_normConsumption;
    else
        m_consumption = fmin(val,BuilderBee::m_normConsumption);

     // увеличение или уменьшение жизненной силы в зависимости от
     // норматива потребления
     double k = m_consumption/BuilderBee::m_normConsumption;
     if(k > 1.0) k = 1.0 + (m_normConsumption - m_consumption)/BuilderBee::m_normConsumption;
     m_currentLiveEnergy *= k;
     m_currentLiveEnergy = fmin(m_currentLiveEnergy, BuilderBee::m_liveEnergyLimit);
}


void BuilderBee::increaseLiveEnergy()
{
    m_currentLiveEnergy += m_consumption;
    if(m_currentLiveEnergy > BuilderBee::m_liveEnergyLimit)
        m_currentLiveEnergy = BuilderBee::m_liveEnergyLimit;
}

void BuilderBee::setAge(int age)
{
    if(age > BuilderBee::m_ageLimit)
        m_age = BuilderBee::m_ageLimit;
    else
        m_age = age;
}
