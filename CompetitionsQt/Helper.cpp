#include "Helper.h"

// Функция сравнения по времени для сортировки списка
int CompareByFinish(Participant one, Participant two)
{
      if(one.startTime > two.endTime) return 1;
      else if(one.startTime < two.endTime) return -1;
      else return 0;
}

// ---- функции сортировки ----
// взаимный обмен значениями между one и two
template <typename T>
void Swap(T &one, T& two)
{
    T swap;
    swap = one;
    one = two;
    two = swap;
}

// сортировка массива по возрастанию
// arr - сортируемый массив
// n - размерность массива
// int (*Compare)(T, T) - указатель на функцию сравнения элеметов массива
// --- Функция сравнения элеметов массива --
// возвращает 1, если первый параметр "больше" второго параметра
// возвращает 0, если первый параметр "равен" второму параметра
// возвращает -1, если первый параметр "меньше" второго параметра
// функция сравнения должна быть разработана отдельно для каждого типа
template<typename T>
void BubbleSortingAsc(T *arr, int n, int (*Compare)(T, T) )
{
    int prev = 0;
    int next = prev + 1;
    while(next < n)
    {
        if(Compare(arr[prev], arr[next]) > 0) // проверка правильнсти сортировки
        {
            // поменять местами элементы массива
            int one = 0, two = one + 1;
            while(two < n)
            {

               if(Compare(arr[one],arr[two] ) > 0)
                   Swap(arr[one], arr[two]);
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
void Reverse(T * arr, int n)
{
    int i = 0, k = 1;
    while(i != n-k)
    {
        Swap(arr[i], arr[n-k]);
        i++; k++;
    }
}
// сортировка массива по возрастанию (asc = true) или по убыванию (asc = false)
// arr - сортируемый массив
// n - размерность массива
// int (*Compare)(T, T) - указатель на функцию сравнения элеметов массива
template <typename T>
void BubbleSorting(T *arr, int n, int (*Compare)(T, T), bool asc = true)
{
    BubbleSortingAsc(arr, n, Compare);
    if(!asc)
        Reverse(arr, n);
}
// ------------------------------------------------------------------------------------------------------------------------

