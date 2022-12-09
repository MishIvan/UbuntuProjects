#ifndef ABEE_H
#define ABEE_H

// статус собирателя (не занят, в рейсе, на восстановлении)
enum CollectorStatus
{
    IDLE = 1, // не занятый
    IN_TRIP,  // в рейсе
    RECREATION // на восстановлении
};

// пчела - базовый класс для всех пчёл
class ABee
{
protected:     
    double m_currentLiveEnergy; // текущее значение жизненной силы (обнуление её означает смерть пчелы)
    double m_consumption; // фактическое потребление, ед. мёда в ед. времени    
    int m_age; // возраст пчелы

public:
    virtual ~ABee();

    // потребление
    double consumption();

    // если установить val в ноль, установка нормативного потребления
    virtual void setConsumption(double val) = 0;

    // увеличить жизненную силу пчелы за ед. времени
    virtual void increaseLiveEnergy() = 0;
    // уменьшить жизненную силу пчелы за ед. времени
    void decreaseLiveEnergy();
    // текущее значение жизненной силы
    double currentLiveEnergy();

    // возраст
    int age();
    virtual void setAge(int age) = 0;

};

// матка
class MotherBee : public ABee
{
public:
    static double m_liveEnergyLimit; // предельное значение жизненной силы
    static int m_ageLimit; // срок жизни в единицах времени
    static double m_normConsumption; // необходимое нормативное потребление

    // параметры
    MotherBee();

    // установить потребление
    void setConsumption(double val);

    // увеличить жизненную силу пчелы за ед. времени
    void increaseLiveEnergy();

    // установить возраст
    void setAge(int age);

};

// пчела сборщик нектара
class CollectorBee : public ABee
{
    CollectorStatus m_Status; // статус собирателя
    double m_normConsumption; // необходимое нормативное потребление

public:
    static double m_liveEnergyLimit; // предельное значение жизненной силы
    static int m_ageLimit; // срок жизни в единицах времени
    static double m_nectarToTrip; // количество нектара, доставляемого пчелой за рейс
    static double m_idleConsumption; // потребление в незанятом состоянии

    // параметры
    // consumption - норматив потребления
    CollectorBee();

    // получить статус собирателя
    CollectorStatus status();

    // установить статус собирателя и его норму потребления
    void setStatus(CollectorStatus stat);

    // установить потребление
    void setConsumption(double val);

    // увеличить жизненную силу пчелы за ед. времени
    void increaseLiveEnergy();

    // установить возраст
    void setAge(int age);

    // норматив потребления
    double normConsumption();

};

// пчела строитель
class BuilderBee  : public ABee
{
public:
    static double m_liveEnergyLimit; // предельное значение жизненной силы
    static int m_ageLimit; // срок жизни в единицах времени
    static double m_normConsumption; // необходимое нормативное потребление

    BuilderBee();

    // количество мёда, переработанное из нектара
    // параметры qnectar - количество перерабатываемого нектара
    double produceHoney(double qnectar);

    // установить потребление
    void setConsumption(double val);

    // увеличить жизненную силу пчелы за ед. времени
    void increaseLiveEnergy();

    // установить возраст
    void setAge(int age);


};




#endif // ABEE_H
