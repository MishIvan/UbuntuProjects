#ifndef SORTING_H
#define SORTING_H
#include <QList>
#include <algorithm>

// ---- функции сортировки ----

// сортировка элементов спсика по возрастанию
// lst - сортируемый список
// Comparsion - указатель на функцию (или лямбда) сравнения элементов списка
// --- Функция сравнения элементов списка --
// возвращает 1, если первый параметр "больше" второго параметра
// возвращает 0, если первый параметр "равен" второму параметра
// возвращает -1, если первый параметр "меньше" второго параметра
// функция сравнения должна быть разработана отдельно для каждого типа
template<typename T, typename Comparsion>
void BubbleSortingAsc(QList<T> &lst, Comparsion fn )
{
    int prev = 0;
    int next = prev + 1;
    int n = lst.size();
    while(next < n)
    {
        if(fn(lst[prev], lst[next]) > 0) // проверка правильнсти сортировки
        {
            // поменять местами элементы массива
            int one = 0, two = one + 1;
            while(two < n)
            {

               if(fn(lst[one],lst[two] ) > 0)
                   lst.swap(one,two);
               one++; two++;
            }
            prev = 0; next = prev + 1;
        }
        else
        {
            prev++; next++;
        }
    }

}

// сортировка элементов списка по возрастанию (asc = true) или по убыванию (asc = false)
// lst - сортируемый список
// Comparsion - указатель на функцию (или лямбда) сравнения элементов списка
template <typename T, typename Comparsion>
void BubbleSorting(QList<T> &lst, Comparsion fn, bool asc = true)
{
    BubbleSortingAsc(lst, fn);
    if(!asc)
        std::reverse(lst.begin(), lst.end());
}
// ------------------------------------------------------------------------------------------------------------------------

#endif // SORTING_H
