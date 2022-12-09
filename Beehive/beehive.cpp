#include "beehive.h"

BeeHive::BeeHive()
{
    m_mother = 0;
    m_isAlive = true;
    m_nectarStore = 0.0;
    m_koeffHoneyStore = 0.8;
    m_honeyStore = 0.0;
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
    cout << "Начальные запасы мёда (ед. количества): ";
    cin >> m_honeyStore;

    cout << "Введите сроки жизни для матки, строителя и собирателя (ед. времени): ";
    cin >> MotherBee::m_ageLimit >> BuilderBee::m_ageLimit >> CollectorBee::m_ageLimit;

    cout << "Введите пределы жизненных сил для матки, строителя и собирателя (ед. количества): ";
    cin >> MotherBee::m_liveEnergyLimit >> BuilderBee::m_liveEnergyLimit >> CollectorBee::m_liveEnergyLimit;

    cout << "Введите нормы потребления для матки, строителя и собирателя (ед. количества в ед. времени)";
    cin >> MotherBee::m_normConsumption >> BuilderBee::m_normConsumption >> CollectorBee::m_idleConsumption;

    cout << "Введите среднее количество нектара, которое приности собиратель за один рейс (ед. количества): ";
    cin >> CollectorBee::m_nectarToTrip;

    // заведение пчёл строителей и собирателей
    int nbuild, ncollect;
    cout << "Введите количество пчёл строителей и собирателей: ";
    cin >> nbuild >> ncollect;
    for(int i=0; i < nbuild; i++)
    {
        BuilderBee bee;
        m_builders.push_back(bee);
    }

    for(int i=0; i < ncollect; i++)
    {
        CollectorBee bee;
        m_collectors.push_back(bee);
    }

    // параметры матки
    m_mother = new MotherBee();
}

void BeeHive::dispatchCollectors(int count)
{
    int n = m_collectors.size();
    for(int i=0; i < n; i++)
    {
        if(m_collectors[i].status() == IDLE)
        {
            m_collectors[i].setStatus(IN_TRIP);
            if(--count < 1) break;
        }
    }

}

double BeeHive::overallConsumption(bool norm)
{
    double val = 0.0;
    // строители постоянно потребляют
    int n = m_builders.size();
    for(int i=0; i < n; i++)
    {
        val += norm ? BuilderBee::m_normConsumption : m_builders[i].consumption();
    }

    // собиратели не потребляют в рейсе
    n = m_collectors.size();
    for(int i=0; i < n; i++)
    {
        double cons = norm ?  m_collectors[i].normConsumption() :m_collectors[i].consumption();
        val += m_collectors[i].status() != IN_TRIP ? cons : 0.0;
    }

    val += norm ? MotherBee::m_normConsumption : m_mother->consumption();
    return val;
}

void BeeHive::correctPopulationQuantity()
{

    bool motherLiveOver = MotherBee::m_ageLimit <= m_mother->age();

    if(m_mother->currentLiveEnergy() < MotherBee::m_liveEnergyLimit*0.01
            || motherLiveOver)
    {
        if(motherLiveOver) // рождение новой матки
        {
            delete m_mother;
            m_mother = new MotherBee();
            m_isAlive = true;
        }
        else
        {
            m_isAlive = false;
            return;
        }
    }

    int i = 0;
    while(i < (int)m_builders.size())
    {
        if(m_builders[i].currentLiveEnergy() < BuilderBee::m_liveEnergyLimit*0.01
                || BuilderBee::m_ageLimit <= m_builders[i].age())
            m_builders.erase(m_builders.begin() + i);
        else
            i++;
    }
    if(m_builders.empty())
    {
        m_isAlive = false;
        return;
    }
    i = 0;
    while(i < (int)m_collectors.size())
    {
        if(m_collectors[i].currentLiveEnergy() < CollectorBee::m_liveEnergyLimit*0.01
                || CollectorBee::m_ageLimit <= m_collectors[i].age())
            m_collectors.erase(m_collectors.begin() + i);
        else
            i++;
    }
    if(m_collectors.empty())
    {
        m_isAlive = false; return;
    }
    cout << "Количество строителей :" << m_builders.size() << endl;
    cout << "Количество собирателей: " << m_collectors.size() << endl;
}

int BeeHive::calculateIdleCollectors()
{
    int q = 0;
    int n = m_collectors.size();
    for(int i = 0; i <n; i++)
    {
        if(m_collectors[i].status() == IDLE) { q++; }
    }
    return q;
}

void BeeHive::setBeesAge(int age)
{
    int n = m_collectors.size();
    for(int i = 0; i <n; i++)
        m_collectors[i].setAge(age);
    n = m_builders.size();
    for(int i = 0; i <n; i++)
        m_builders[i].setAge(age);
    m_mother->setAge(age);

}

void BeeHive::generateWorkingBees(int ncollectors, int nbuilders)
{
    for(int i = 0; i < ncollectors; i++)
    {
        CollectorBee bee;
        m_collectors.push_back(bee);
    }

    for(int i = 0; i < nbuilders; i++)
    {
        BuilderBee bee;
        m_builders.push_back(bee);
    }
}

void BeeHive::step()
{

    // добыча нектара
    int n = m_collectors.size();
    double nectarq = 0.0;
    for(int i = 0; i <n; i++)
    {
        if(m_collectors[i].status() == IN_TRIP)
        {
            nectarq += CollectorBee::m_nectarToTrip;
            m_collectors[i].decreaseLiveEnergy();
            m_collectors[i].setStatus(RECREATION);
            m_collectors[i].setConsumption(0.0);
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
         qhoney += m_builders[i].produceHoney(qn);
         m_builders[i].decreaseLiveEnergy();
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
          double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
              m_honeyStore*m_koeffHoneyStore*m_collectors[i].consumption()/overall :
                   m_collectors[i].normConsumption();
           CollectorStatus st = m_collectors[i].status();

           if(st != IN_TRIP)
           {
                m_collectors[i].setConsumption(cons); // потребление
                m_collectors[i].increaseLiveEnergy(); // увеличение жизненной энергии
            }

             //проверка, произошло ли восстановление жизненных сил собирателя
             if(st == RECREATION)
             {
                 //  готова ли прела-осбиратель к отправке в рейс
                 if(m_collectors[i].currentLiveEnergy() >= CollectorBee::m_liveEnergyLimit*0.85)
                       m_collectors[i].setStatus(IDLE);
             }

       }

      n = m_builders.size();
       for(int i = 0; i <n; i++)
       {
           double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
               m_honeyStore*m_koeffHoneyStore*m_builders[i].consumption()/overall :
                    BuilderBee::m_normConsumption;
           m_builders[i].setConsumption(cons); // потребление
           m_builders[i].increaseLiveEnergy(); // увеличение жизненной энергии

       }

       double cons = m_honeyStore*m_koeffHoneyStore < overallNorm ?
                   m_honeyStore*m_koeffHoneyStore*m_mother->consumption()/overall :
                   MotherBee::m_normConsumption;
       m_mother->setConsumption(cons); // потребление
       m_mother->increaseLiveEnergy(); // увеличение жизненной энергии

       m_honeyStore -= overall;
       cout << "Запасы мёда: " << m_honeyStore << endl;

       // перевести собирателей, бывших в рейсе на отдых
       n = m_collectors.size();
       for(int i = 0; i <n; i++)
       {

              if(m_collectors[i].status() == IN_TRIP)
                  m_collectors[i].setStatus(RECREATION);
        }

}
