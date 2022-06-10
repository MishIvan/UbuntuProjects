#include <iostream>
#include <string>
#include <Helper.h>

using namespace std;

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

int main()
{
    setlocale(LC_ALL, "");
    string age_groups[3] = {"от 18 до 30 лет", "от 31 до 55 лет", "от 56 до 75 лет"};

    vector <Participant> Groups[3];
    // ввод данных об участниках соревнований с консоли и распределение их по возрастным группам
    InputDataFromConsole(Groups[0], Groups[1], Groups[2]);
    for(int i =0 ; i < 3 ; i++)
    {
        if(Groups[i].size() < 1) continue;
        Participant * part = Groups[i].data();

        // сортировка списка участников каждой из групп по времени финиша
        BubbleSorting(part, Groups[i].size(), CompareByFinish);

        // вывод результатов на консоль
        cout << "Участники возрастной группы " << age_groups[i]  << endl;
        OutputParticipantsToConsole(Groups[i], 0);
        cout << "Чемпион возрастной группы " << age_groups[i]  << endl;
        OutputParticipantsToConsole(Groups[i], 1);
    }


    return 0;
}

// --- функции для теста правильности сортировки ---
int Compare(int one , int two)
{
    if(one > two) return 1;
    else if(one == two) return 0;
    else return -1;
}

void test_int()
{
    cout << "Пузырьковая сортировка" << endl;
    int arr[] = {22, 8, 8, 8, 99, 7, 1078, 9, 33, 87, 41};
    int size = sizeof(arr)/sizeof(arr[0]);
    string s1;
    bool asc = false;
    if(asc)
        s1 ="возрастанию";
    else
        s1 ="убыванию";

    cout << "Массив, сортированный по " << s1<<  " элементов" << endl;
    BubbleSorting(arr, size, Compare, asc);
    for(int a : arr)
        cout << a << " ";
    cout << endl;

}

