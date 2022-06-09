#ifndef HELPER_H
#define HELPER_H
#include<iostream>
#include <vector>

bool ParseTime(std::string , time_t & );

// участник соревнований
struct Participant
{
    std::string Name; // ФИО участника
    time_t TimeBegin; // Время старта
    time_t TimeEnd; // Время финиша
    int Age; // возраст участника
};

void InputDataFromConsole(std::vector<Participant> & ,  std::vector <Participant> & , std::vector<Participant> & );
int CompareByFinish(Participant , Participant );
void OutputParticipantsToConsole(std::vector<Participant> & , int);

#endif // HELPER_H
