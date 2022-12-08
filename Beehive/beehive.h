// класс, реализующий модель улья
#ifndef BEEHIVE_H
#define BEEHIVE_H
#include "abee.h"
#include <vector>
#include <iostream>

using namespace std;

class BeeHive
{
    double m_honeyStore;  // запасы мёда
    bool m_isAlive;              // жизнеспособен ли улей (наличие запасов мёда, жива ли матка)
    double m_nectarStore;  // запасы нектара
    double m_koeffHoneyStore; // коэффициен сохранения гарантированного запаса мёда (меньше единицы)

    // время жизни матки, строителя и собирателя
    int m_motherLiveTime;
    int m_builderLiveTime;
    int m_collectorLiveTime;

    // предел жизненных сил матки, строителя и собирателя
    double m_motherLiveEnergy;
    double m_builderLiveEnergy;
    double m_collectorLiveEnergy;

    // норматив потребления для поддержания жизненных сил
    // без их снижения
    // для матки, строителя и собирателя
    double m_motherConsumption;
    double m_builderConsumption;
    double m_collectorConsumption;

    // среднее количество нектара, которое несёт собиратель за рейс
    double m_nectarPerTrip;

    vector <CollectorBee> m_collectors; // собиратели
    vector <BuilderBee> m_builders;      // строители
    MotherBee *m_mother;                     // матка

    // общее потребление для всего улья
    // norm - фактическое (false), нормативное (true)
    double overallConsumption(bool norm = false);
public:
    BeeHive();
    ~BeeHive();

    // ввод параметров модели и заведение пчёл в улье
    // функция вызывается сразу же после вызова конструктора
    void populate();
    // скорректировать численность пчёл
    void correctPopulationQuantity();
    // расчёт параметров пчёл за единицу времени
    void step();
    // жизнеспособен ли улей
    bool isAlive();
    // определить количество незанятых собирателей
    int calculateIdleCollectors();
};

#endif // BEEHIVE_H
