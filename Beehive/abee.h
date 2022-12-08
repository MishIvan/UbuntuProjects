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
    double m_liveEnergyLimit; // предельное значение жизненной силы
    double m_currentLiveEnergy; // текущее значение жизненной силы (обнуление её означает смерть пчелы)
    int m_liveTimeLimit; // срок жизни в единицах времени
    double m_consumption; // фактическое потребление, ед. мёда в ед. времени
    double m_normConsumption; // необходимое нормативное потребление

public:
    virtual ~ABee();
    // параметры
    // live_limit - предел жизненных сил
    // consumption - норматив потребления
    // time_limit - продолжительность жизни
    ABee(double live_limit, double consumption, int time_limit);

    // значение предела жизненной силы
    double liveEnergyLimit();

    // значение предела жизненной силы
    int liveTimeLimit();

    // нормативное потребление
    double normConsumption();

    // потребление
    double consumption();
    // если установить val в ноль, установка нормативного потребления
    void setConsumption(double val);

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

    // порождение новой матки
    MotherBee * clone();
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

    // получить статус собирателя
    CollectorStatus status();

    // установить статус собирателя и его норму потребления
    void setStatus(CollectorStatus stat,double builderConsumption);

    // получить количество нектара, которое приносит собиратель за рейс
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
