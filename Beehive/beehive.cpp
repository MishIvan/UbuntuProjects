#include "beehive.h"

BeeHive::BeeHive()
{
    m_mother = 0;
    m_isAlive = true;
    m_nectarStore = 0.0;
    m_koeffHoneyStore = 0.8;
}

BeeHive::~BeeHive()
{
    m_builders.clear();
    m_collectors.clear();
    if(m_mother!=0) delete m_mother;
}

bool BeeHive::isAlive()
{ return m_isAlive;}

void BeeHive::populate()
{
    cout << "Введите сроки жизни для матки, строителя и собирателя (ед. времени): ";
    cin >> m_motherLiveTime >> m_builderLiveTime >> m_collectorLiveTime;

    cout << "Введите пределы жизненных сил для матки, строителя и собирателя (ед. количества мёда): ";
    cin >> m_motherLiveEnergy >> m_builderLiveEnergy >> m_collectorLiveEnergy;

    cout << "Введите нормы потребления для матки, строителя и собирателя (ед. количества мёда в ед. времени)";
    cin >> m_motherConsumption >> m_builderConsumption >> m_collectorConsumption;

    cout << "Введите среднее количество нектара, которое приности собиратель за один рейс (ед. нектара): ";
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

double BeeHive::overallConsumption(bool norm)
{
    double val = 0.0;
    // строители постоянно потребляют
    int n = m_builders.size();
    for(int i=0; i < n; i++)
    {
        BuilderBee bee = m_builders[i];

        val += norm ? bee.normConsumption() : bee.consumption();
    }

    // собиратели не потребляют в рейсе
    n = m_collectors.size();
    for(int i=0; i < n; i++)
    {
        CollectorBee bee = m_collectors[i];
        double cons = norm ?  bee.normConsumption() :bee.consumption();
        val += m_collectors[i].status() != IN_TRIP ? cons : 0.0;
    }

    val += norm ? m_mother->normConsumption() : m_mother->consumption();
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

int BeeHive::calculateIdleCollectors()
{
    int q = 0;
    int n = m_collectors.size();
    for(int i = 0; i <n; i++)
    {
        if(m_collectors[i].status() == IDLE) q++;
    }
    return q;
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
        {
            nectarq += bee.nectarToTrip();
            bee.decreaseLiveEnergy();
            bee.setStatus(RECREATION, m_builderConsumption);
            bee.setConsumption(0.0);
        }
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
         bee.decreaseLiveEnergy();
         m_nectarStore -= qn;
     }
     m_honeyStore += qhoney; // увеличение запасов

     // вывод результатов за ед. времени
     cout << "Собрано нектара: " << nectarq << endl;
     cout << "Выбаротано мёда: " << qhoney << endl;
     cout << "Запасы нектара: " << m_nectarStore << endl;
     cout << "Запасы мёда: " << m_honeyStore << endl << endl;
     cout << "Потребление запасов" << endl;

     // потребление запасов для жизнедеятельности
     double overallNorm = overallConsumption(true);

     // установить норматив потребления
      double overall = overallConsumption(false);

      // собиратели
      n = m_collectors.size();
      for(int i = 0; i <n; i++)
      {
          CollectorBee bee = m_collectors[i];
          double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
              m_honeyStore*m_koeffHoneyStore*bee.consumption()/overall :
                   bee.normConsumption();
           CollectorStatus st = bee.status();

           if(st != IN_TRIP)
           {
                bee.setConsumption(cons); // потребление
                bee.increaseLiveEnergy(); // увеличение жизненной энергии
            }

             //проверка, произошло ли восстановление жизненных сил собирателя
             if(st == RECREATION)
             {
                 //  готова ли прела-осбиратель к отправке в рейс
                 if(bee.currentLiveEnergy() >= bee.liveEnergyLimit()*0.85)
                       bee.setStatus(IDLE, m_builderConsumption);
             }
       }

      n = m_builders.size();
       for(int i = 0; i <n; i++)
       {
           BuilderBee bee = m_builders[i];
           double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
               m_honeyStore*m_koeffHoneyStore*bee.consumption()/overall :
                    bee.normConsumption();
           bee.setConsumption(cons); // потребление
           bee.increaseLiveEnergy(); // увеличение жизненной энергии

       }

       double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
                   m_honeyStore*m_koeffHoneyStore*m_mother->consumption()/overall :
                   m_mother->normConsumption();
       m_mother->setConsumption(cons); // потребление
       m_mother->increaseLiveEnergy(); // увеличение жизненной энергии

       m_honeyStore -= overall;
       cout << "Запасы мёда: " << m_honeyStore << endl;
}
