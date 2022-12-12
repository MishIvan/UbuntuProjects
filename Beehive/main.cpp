#include <iostream>
#include "beehive.h"

using namespace std;

int CollectorBee::m_ageLimit = 0;
double CollectorBee::m_idleConsumption = 0.0;
double CollectorBee::m_liveEnergyLimit = 0.0;
double CollectorBee::m_nectarToTrip;

int BuilderBee::m_ageLimit = 0;
double BuilderBee::m_normConsumption = 0.0;
double BuilderBee::m_liveEnergyLimit = 0.0;

int MotherBee::m_ageLimit = 0;
double MotherBee::m_normConsumption = 0.0;
double MotherBee::m_liveEnergyLimit = 0.0;

int main()
{
    BeeHive hive;
    hive.populate();
    char ch, ch1;
    do
    {
        hive.correctPopulationQuantity();
        if(!hive.isAlive())
        {
            cout << "Количество населения улья недостаточно для поддержания его существования" << endl;
            break;
        }

        int qc = hive.calculateIdleCollectors();
        cout << "Количество незанятых собирателей: " << qc << endl;
        int qci = 0;
        do {
            cout << "Введите число незанятых собирателей (число не более " << qc << " ) для отправки в рейс: ";
            cin >> qci;
        }  while (qci > qc);
        hive.dispatchCollectors(qci);
        hive.step();
        hive.setBeesAge();
        cout << "Продолжить (y/n)? ";
        cin >> ch;

        if(ch == 'y' || ch =='Y')
        {
            cout << "Породить рабочих пчёл (y/n)? ";
            cin >> ch1;
            if(ch1 == 'y' || ch1 =='Y')
            {
                int nb = 0, nc = 0;
                cout << "Введите число собирателей и число строителей для порождения: ";
                cin >> nc >> nb;
                hive.generateWorkingBees(nc, nb);
            }
        }

    } while(ch == 'y' || ch =='Y');
    return 0;
}
