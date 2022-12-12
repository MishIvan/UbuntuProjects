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

    // назначить собирателей в рейс из числа незанятых
    // параметры: count - количество незанятых собирателей
    void dispatchCollectors(int count);

    // увеличить возраст пчёл
    void setBeesAge();

    // породить рабочих пчёл: собирателей и строителей
    // параметры: ncollectors - порождать собирателей
    // параметры: nbuilders - порождать строителей
    void generateWorkingBees(int ncollectors, int nbuilders);
};

#endif // BEEHIVE_H
