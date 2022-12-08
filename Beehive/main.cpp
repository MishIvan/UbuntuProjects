#include <iostream>
#include "beehive.h"

using namespace std;

int main()
{
    BeeHive hive;
    hive.populate();
    cout << "Введите число шагов: ";
    int nsteps = 0;
    cin >> nsteps;
    for(int i=0; i < nsteps; i++)
    {
        hive.correctPopulationQuantity();
        if(!hive.isAlive())
        {
            cout << "Количество населения улья недостаточно для поддержания его существования" << endl;
            break;
        }

        int qc = hive.calculateIdleCollectors();
        int qci = 0;
        do {
            cout << "Введите число незанятых собирателей (число не более " << qc << " )";
            cin >> qci;
        }  while (qci > qc);
        hive.step();
    }
    return 0;
}
