#ifndef SORTING_H
#define SORTING_H
#include <QList>

// ---- функции сортировки ----

// сортировка массива по возрастанию
// lst - сортируемый список
// int (*Compare)(T, T) - указатель на функцию сравнения элеметов массива
// --- Функция сравнения элеметов массива --
// возвращает 1, если первый параметр "больше" второго параметра
// возвращает 0, если первый параметр "равен" второму параметра
// возвращает -1, если первый параметр "меньше" второго параметра
// функция сравнения должна быть разработана отдельно для каждого типа
template<typename T>
void BubbleSortingAsc(QList<T> &lst, int (*Compare)(T, T) )
{
    int prev = 0;
    int next = prev + 1;
    int n = lst.size();
    while(next < n)
    {
        if(Compare(lst[prev], lst[next]) > 0) // проверка правильнсти сортировки
        {
            // поменять местами элементы массива
            int one = 0, two = one + 1;
            while(two < n)
            {

               if(Compare(lst[one],lst[two] ) > 0)
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

// перестановка элементов массива в обратном порядке
// первый элемент становится последним, а последний первым
template <typename T>
void Reverse(QList<T> &lst)
{
    int i = 0, k = 1;
    int n = lst.size();
    while(i != n-k)
    {
        lst.swap(i, n-k);
        i++; k++;
    }
}
// сортировка массива по возрастанию (asc = true) или по убыванию (asc = false)
// lst - сортируемый список
// n - размерность массива
// int (*Compare)(T, T) - указатель на функцию сравнения элеметов массива
template <typename T>
void BubbleSorting(T *arr, int (*Compare)(T, T), bool asc = true)
{
    BubbleSortingAsc(arr, Compare);
    if(!asc)
        Reverse(arr);
}
// ------------------------------------------------------------------------------------------------------------------------

#endif // SORTING_H
