// класс, реализующий модель улья
#ifndef BEEHIVE_H
#define BEEHIVE_H
#include "abee.h"
#include <vector>
#include <iostream>

using namespace std;

class BeeHive
{
    double m_honeyStore; // запасы мёда
    bool m_isAlive; // жизнеспособен ли улей (наличие запасов мёда, жива ли матка)
    double m_nectarStore; // запасы нектара

    // время жизни матки, строителя и собирателя
    int m_motherLiveTime;
    int m_builderLiveTime;
    int m_collectorLiveTime;

    // предел жизненных сил матки, строителя и собирателя
    double m_motherLiveEnergy;
    double m_builderLiveEnergy;
    double m_collectorLiveEnergy;

    // норматив потребления для поддержания жизненных сил
    // для матки, строителя и собирателя
    double m_motherConsumption;
    double m_builderConsumption;
    double m_collectorConsumption;

    // среднее количество нектара, которое несёт собиратель за рейс
    double m_nectarPerTrip;

    vector <CollectorBee> m_collectors;
    vector <BuilderBee> m_builders;
    MotherBee *m_mother;

    // общее потребление для всего улья
    double overallConsumption();
    // скорректировать численность пчёл
    void correctPopulationQuantity();
public:
    BeeHive();
    ~BeeHive();

    // ввод параметров модели и заведение пчёл в улье
    // функция вызывается сразу же после вызова конструктора
    void populate();
    // вычисление потребностей в нектаре для поддержания запасов мёда
    // и определение жизненных сил пчёл улья
    void calculateNeeds();
    // расчёт параметров пчёл за единицу времени
    void step();
    //
    bool isAlive();
};

#endif // BEEHIVE_H
