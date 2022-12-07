#include "beehive.h"

BeeHive::BeeHive()
{
    m_mother = 0;
    m_isAlive = true;
    m_nectarStore = 0.0;
}

BeeHive::~BeeHive()
{
    m_builders.clear();
    m_collectors.clear();
    if(m_mother!=0) delete m_mother;
}

void BeeHive::populate()
{
    cout << "Введите сроки жизни для матки, строителя и собирателя (ед. времени): ";
    cin >> m_motherLiveTime >> m_builderLiveTime >> m_collectorLiveTime;

    cout << "Введите пределы жизненных сил для матки, строителя и собирателя (ед. количества мёда): ";
    cin >> m_motherLiveEnergy >> m_builderLiveEnergy >> m_collectorLiveEnergy;

    cout << "Введите нормы потребления для матки, строителя и собирателя (ед. количества мёда в ед. времени)";
    cin >> m_motherConsumption >> m_builderConsumption >> m_collectorConsumption;

    cout << "Введите среднее количество нектара, которое приности собиратель за один рейс (ед. нектара в ед. времени): ";
    cin >> m_nectarPerTrip;

    // заведение пчёл строителей и собирателей
    int nbuild, ncollect;
    cout << "Введите количество пчёл строителей и собирателей: ";
    cin >> nbuild >> ncollect;
    for(int i=0; i < nbuild; i++)
    {
        BuilderBee bee(m_builderLiveEnergy, m_builderConsumption, m_builderLiveTime);
        m_builders.push_back(bee);
    }

    for(int i=0; i < ncollect; i++)
    {
        CollectorBee bee(m_collectorLiveEnergy, m_collectorConsumption,
                         m_collectorLiveTime, m_nectarPerTrip);
        m_collectors.push_back(bee);
    }

    // параметры матки
    m_mother = new MotherBee(m_motherLiveEnergy, m_motherConsumption, m_motherLiveTime);
}

double BeeHive::overallConsumption()
{
    double val = 0.0;
    // строители постоянно потребляют
    int n = m_builders.size();
    for(int i=0; i < n; i++)
    {

        val += m_builders[i].consumption();
    }

    // собиратели не потребляют в рейсе
    n = m_collectors.size();
    for(int i=0; i < n; i++)
    {
        val += m_collectors[i].status() != IN_TRIP ? m_collectors[i].consumption() : 0.0;
    }

    val += m_mother->consumption();
    return val;
}

void BeeHive::correctPopulationQuantity()
{

    if(m_mother->currentLiveEnergy() < m_mother->liveEnergyLimit()*0.01
            || m_mother->liveTimeLimit() < m_motherLiveTime)
    {
        delete m_mother;
        m_mother = 0;
        m_isAlive = false;
        return;
    }

    int i = 0;
    while(i < (int)m_builders.size())
    {
        if(m_builders[i].currentLiveEnergy() < m_builders[i].liveEnergyLimit()*0.01
                || m_builders[i].liveTimeLimit() <= 0)
            m_builders.erase(m_builders.begin() + i);
        else
            i++;
    }
    if(m_builders.size() < 1)
    {
        m_isAlive = false;
        return;
    }
    i = 0;
    while(i < (int)m_collectors.size())
    {
        if(m_collectors[i].currentLiveEnergy() < m_collectors[i].liveEnergyLimit()*0.01
                || m_collectors[i].liveTimeLimit() <= 0)
            m_collectors.erase(m_collectors.begin() + i);
        else
            i++;
    }
    if(m_collectors.size() < 1)
        m_isAlive = false;

}

void BeeHive::calculateNeeds()
{
    // определить численность пчёл
    // рассчитать общий расход мёда

}

void BeeHive::step()
{

    // добыча нектара
    int n = m_collectors.size();
    double nectarq = 0.0;
    for(int i = 0; i <n; i++)
    {
        CollectorBee bee = m_collectors[i];
        if(bee.status() == IN_TRIP)
            nectarq += bee.nectarToTrip();
    }
    m_nectarStore += nectarq;

    // выработка мёда
    n = m_builders.size();
    double qn = m_nectarStore/n; // сколько нектара приходится на одну пчелу-строителя
    if(qn > 10.0) // пчела-строитель перерабатывает максимум 10 ед. нектара
       qn = 10.0;
     double qhoney = 0.0;
     for(int i = 0; i <n; i++)
     {
         BuilderBee bee = m_builders[i];
         qhoney += bee.produceHoney(qn);
         m_nectarStore -= qn;
     }
     m_honeyStore += qhoney;
     m_honeyStore -= overallConsumption(); // потребление запасов

     // вывод результатов за ед. времени
     cout << "Собрано нектара: " << nectarq << endl;
     cout << "Выбаротано мёда: " << qhoney << endl;
     cout << "Запасы нектара: " << m_nectarStore << endl;
     cout << "Выбаротано мёда: " << m_honeyStore << endl;

}
