#include <iostream>
#include <string>
#include <ctime>
#include <Helper.h>

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
    setlocale(LC_ALL, "");
    string age_groups[3] = {"от 18 до 30", "от 31 до 55", "от 56 до 75"};

//    string fio;
//    cout << "Введите ФИО участника: ";
//    getline(cin,fio );

//    int age = 0;
//    do {
//        cout << "Введите возраст участника: ";
//        cin >> age;
//      if(age < 18 || age > 75)
//        cerr << "Возраст должен находиться в пределах от 18 до 75 лет." << endl;
//    } while(age < 18 || age > 75);


    vector <Participant> Groups[3];
    InputDataFromConsole(Groups[0], Groups[1], Groups[2]);
    for(int i =0 ; i < 3 ; i++)
    {
        Participant * part = Groups[i].data();
        BubbleSorting(part, Groups[i].size(), CompareByFinish);
        cout << "Участники возрастной группы " << age_groups[i] <<" лет" << endl;
        OutputParticipantsToConsole(Groups[0], 0);
        cout << "Призёры возрастной группы " << age_groups[i] <<" лет" << endl;
        OutputParticipantsToConsole(Groups[0], 3);
    }


    return 0;
}

void test()
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
