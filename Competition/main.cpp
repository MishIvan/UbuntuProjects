#include <iostream>

using namespace std;
int Compare(int one , int two)
{
    if(one > two) return 1;
    else if(one == two) return 0;
    else return -1;
}
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

template <typename T>
void BubbleSorting(T *arr, int n, int (*Compare)(T, T), bool asc = true)
{
    BubbleSortingAsc(arr, n, Compare);
    if(!asc)
        Reverse(arr, n);
}

int main()
{
    cout << "Пузырьковая сортировка" << endl;
    int arr[] = {22, 8, 8, 8, 99, 7, 1078, 9, 33, 87, 41};
    int size = sizeof(arr)/sizeof(arr[0]);
    string s1;
    bool asc = true;
    if(asc)
        s1 ="возрастанию";
    else
        s1 ="убыванию";

    cout << "Массив, сортированный по " << s1<<  " элементов" << endl;
    BubbleSorting(arr, size, Compare, asc);
    for(int a : arr)
        cout << a << " ";
    cout << endl;

    return 0;
}
