#ifndef ABEE_H
#define ABEE_H

// статус собирателя (незадействован, в рейсе, на восстановлении)
enum CollectorStatus
{
    IDLE = 1, IN_TRIP, RECREATION
};

// пчела - базовый класс для всех пчёл
class ABee
{
protected:
    // предельное значение жизненной силы,
    // обнуление её означает смерть пчелы
    double m_liveEnergyLimit;
    int m_liveTimeLimit; // срок жизни в единицах времени
    double m_consumption; // потребление, ед. мёда в ед. времени
    double m_currentLiveEnergy; // текущее значение жизненной силы
public:
    virtual ~ABee();
    // параметры live_limit - предел жизненных сил
    // live_limit - предел жизненных сил
    // consumption - норматив потребления
    // time_limit - продолжительность жизни
    ABee(double live_limit, double consumption, int time_limit);

    // значение предела жизненной силы
    double liveEnergyLimit();
    void setLiveEnergyLimit(double val);

    // значение предела жизненной силы
    int liveTimeLimit();
    void setLiveTimeLimit(int val);

    // потребление
    double consumption();
    virtual void setConsumption(double val);

    // увеличить жизненную силу пчелы за ед. времени
    void increaseLiveEnergy();
    // уменьшить жизненную силу пчелы за ед. времени
    void decreaseLiveEnergy();
    // текущее значение жизненной силы
    double currentLiveEnergy();

};

// матка
class MotherBee : public ABee
{
public:

    // параметры live_limit - предел жизненных сил
    // live_limit - предел жизненных сил
    // consumption - норматив потребления
    // time_limit - продолжительность жизни
    MotherBee(double live_limit, double consumption,int time_limit);
};

// пчела сборщик нектара
class CollectorBee : public ABee
{
    CollectorStatus m_Status;
    double m_idleConsumption;
    double m_nectarToTrip;
public:

    // параметры live_limit - предел жизненных сил
    // live_limit - предел жизненных сил
    // consumption - норматив потребления
    // time_limit - продолжительность жизни
    // nectarToTrip - количество нектара, переносимое за рейс
    CollectorBee(double live_limit, double consumption, double liveEnergy, double nectarToTrip);
    CollectorStatus status();
    void setStatus(CollectorStatus stat);
    void setConsumption(double builderConsumption);
    double nectarToTrip();
};

// пчела строитель
class BuilderBee  : public ABee
{
public:
    // параметры live_limit - предел жизненных сил
    // consumption - норматив потребления
    // time_limit - продолжительность жизни
    BuilderBee(double live_limit, double consumption,int time_limit);
    // количество мёда, переработанное из нектара
    // параметры qnectar - количество перерабатываемого нектара
    double produceHoney(double qnectar);
};

#endif // ABEE_H
