// MatrixDet.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <math.h>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <random>

void GetFullPathInWD(char* fullExePath, const char* dataFileName, char* fullFileName);
void LinearSystemSolve(int n, double* A, double* b, double* &x, double& det, bool onlyDet);
bool ReadData(const char* fullFileName, double* &A, double* &b, int& size, bool onlyDet);
double errNorm(int n, double* A, double* b, double* x);
void PrintMatrix(const char* header, int size, double* A);
void PrintVector(const char* header, int size, double* x);
double Minor(int n, double* A, int i, int j);
void CalculateMinors(double *A, int size, double *MMinor);
double MaxMinor(double *minor, int size, int &k, int &p);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");
    char path[1024]; // буфер пути файла данных
    double* A; // задаваемая матрирца
    double* b = nullptr; // вектор правой части (при вычислении определителя задавать не нужно)
    double* x = nullptr; // возвращаемый вектор решения(при вычислении определителя задавать не нужно)
    double norm, det;
    int size = 0;
    bool onlyDet = false;
    GetFullPathInWD(argv[0], "./MatrixT1.txt", path);
    if(!ReadData(path, A, b, size, onlyDet)) return -1;
    PrintMatrix("Матрица А", size, A);
    LinearSystemSolve(size, A, b, x, det, onlyDet);
    if (!onlyDet)
    {
        PrintVector("Вектор b", size, b);
        PrintVector("Решение", size, x);
        norm = errNorm(size, A, b, x);
        std::cout << "Норма ошибки " << norm << std::endl;
        delete x;
        delete b;
    }
    std::cout << "Детерминант матрицы А = " << det << std::endl;
    int i, j;
    int imax = -1, jmax = -1;
    double meanval = 0.0, maxval = -1.7E+308, minor = 0.0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            minor = Minor(size, A, i, j);

            meanval += minor;
            if (minor > maxval)
            {
                imax = i; jmax = j;
                maxval = minor;
            }
        }
    }
    meanval /= (double)size * (double)size;

    double * minors = new double[size*size];
    CalculateMinors(A, size, minors);
    PrintMatrix("Матрица миноров матрицы А", size, minors);
    int k,p;
    double mmax = MaxMinor(minors, size, k,p);
    std::cout << "Максимальный минор M("<< k << "," << p << ") = " << mmax <<" методом отжига" << std::endl;

    delete [] minors;
    delete [] A;

    std::cout << "Среднее значение миноров матрицы A = " << meanval << std::endl;
    std::cout << "Максимальное значение минора M(" << imax + 1 <<","<< jmax + 1 <<") = " << maxval << std::endl;
}

/// <summary>
/// Получить полный путь файла в папке, из которой запускается исполняемый файл программы
/// </summary>
/// <param name="fullExePath">полный путь запуска программы</param>
/// <param name="dataFileName">имя файла данных в каталоге, где расположен исполняемый файл</param>
/// <param name="fullFileName">имя файла данных в каталоге, где расположен исполняемый файл</param>
/// <returns></returns>
void GetFullPathInWD(char* fullExePath, const char* dataFileName, char* fullFileName)
{
    strcpy(fullFileName, fullExePath);
    std::string s1 = fullFileName;
    int k = s1.find_last_of('/');
    if (k == std::string::npos)
        strcpy(fullFileName, dataFileName);
    else
    {
        s1.replace(k + 1, s1.size() - 1, dataFileName);
        strcpy(fullFileName, s1.data());
    }
}

/// <summary>
///Формирование матрицы миноров
/// </summary>
/// <param name="size">размерность исходной матрицы и матрицы миноров</param>
/// <param name="A">матрица, для которой формируются миноры</param>
/// <param name="MMinor">формируемая матрица миноров, память должна быть выделена</param>
void CalculateMinors(double *A, int size, double *MMinor)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            MMinor[i*size+j] = Minor(size, A, i, j);

        }
    }

}

/// <summary>
/// Генератор случайных целых чисел
/// </summary>
/// <param name="from">начальное значение</param>
/// <param name="to">конечное значение</param>
/// <returns>случайное число</returns>

double MaxMinor(double *minor, int size, int &k, int &p)
{
    double Tmax = 1000, Tmin = 1;
    double T = Tmax;
    k = rand()%(size-1);
    p = rand()%(size-1);
    double val0 = minor[k*size+k];

    while(T >= Tmin)
    {
        int i = rand()%(size-1);
        int j = rand()%(size-1);
        double val = minor[i*size+j];
        if(val > val0 || double(rand()) / RAND_MAX < exp(-(val - val0)/T))
        {
            val0 = val; k = i; p = j;
        }
        T *= 0.9;
    }
    return val0;
}

/// <summary>
/// Решение системы линейных уравнений с параллельным вычислением демерминанта матрицы
/// </summary>
/// <param name="n">размерность системы уравнений</param>
/// <param name="A">матрица системы уравнений</param>
/// <param name="b">вектор правой части системы уравнений</param>
/// <param name="x">решение системы уравнений</param>
/// <param name="det">определитель матрицы A</param>
/// <param name="onlyDet">true - рассчитать только детернминант, false - решать систему и рассичтать детерминант</param>
void LinearSystemSolve(int n, double* A, double* b, double* &x, double& det, bool onlyDet)
{
    double *alpha = new double [n*n];

    for (int i = 0; i < n; i++)
    {
        alpha[i * n] = A[i * n];
        if(i > 0) alpha[i] = A[i] / A[0];
    }

    double sum = 0.0;
    int k = 1, i = 1;
    while (i < n)
    {
        if (k >= n)
        {
            k = 1; i++;
            if (i >= n) break;
        }
        if (i >= k)
        {
            sum = 0.0;
            for (int j = 0; j <= k - 1; j++)
            {
                sum += alpha[i*n+j] * alpha[j*n+k];
            }
            alpha[i*n+k] = A[i*n+k] - sum;
        }
        else
        {
            sum = 0.0;
            for (int j = 0; j <= i - 1; j++)
            {
                sum += alpha[i*n+j] * alpha[j*n+k];
            }
            if (alpha[i*n+i] == 0.0)
            {
                delete[] alpha;
                det = 0.0; return;
            }
            alpha[i*n+k] = (A[i*n+k] - sum) / alpha[i*n+i];
        }
        k++;
    }

    if(!onlyDet)
    {
        double* beta = new double[n];
        x = new double[n];
        beta[0] = b[0] / A[0];
        for (int i = 1; i < n; i++)
        {
            sum = 0.0;
            for (int j = 0; j <= i - 1; j++)
                sum += alpha[i*n+j] * beta[j];
            beta[i] = (b[i] - sum) / alpha[i*n+i];
        }

        // решение системы уравнений с труегольной матрицей
        x[n - 1] = beta[n - 1];
        for (int i = n - 2; i >= 0 ; i--)
        {
            sum = 0.0;
            for (int j = n-1; j > i; j--)
                sum += alpha[i*n+j] * x[j];
            x[i] = beta[i] - sum;
        }
        delete[] beta;
    }
    det = 1.0;
    for (int i = 0; i < n; i++)
        det *= alpha[i*n+i];
    delete[] alpha;
}

/// <summary>
///  Евклидова норма погрешности вычисления
/// </summary>
/// <param name="n">размерность</param>
/// <param name="A">матрица системы линейных уравнений</param>
/// <param name="b">вектор правой части системы линейных уравнений</param>
/// <param name="x">вектор решения</param>
/// <returns>евклидова норма погрешности вычислений</returns>
double errNorm(int n, double* A, double *b, double* x)
{
    double qnorm = 0.0;
    for (int i = 0; i < n; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < n; j++)
        {
            sum += A[i*n+j] * x[j];
        }
        qnorm += (sum - b[i])* (sum - b[i]);
    }
    return sqrt(qnorm);
}
/// <summary>
/// Чтение данных из файла
/// </summary>
/// <param name="fullFileName">полное имя фйла</param>
/// <param name="A">матрица</param>
/// <param name="b">вектор</param>
/// <param name="size">размерность</param>
bool ReadData(const char* fullFileName, double* &A, double* &b, int& size, bool onlyDet)
{
    std::fstream inp;
    inp.open(fullFileName, std::ios::in);
    // динамический массив
    if (inp.is_open())
    {
        try
        {
            inp >> size;
            A = new double [size*size];
            b = new double[size];
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                    inp >> A[i*size+j];
            }
            if (!onlyDet)
            {
                for (int j = 0; j < size; j++)
                    inp >> b[j];

            }
        }
        catch (...)
        {
            inp.close();
            std::cout << "Неверный формат файла " << fullFileName;
            return false;

        }

    inp.close();
    return true;
    }
    else
    {
        std::cout << "Неверный формат файла " << fullFileName << " или файл не найден";
        return false;
    }
}
/// <summary>
/// Вывод матрицы на консоль
/// </summary>
/// <param name="header">Заголовок</param>
/// <param name="size">размерность</param>
/// <param name="A">матрица для вывода на консоль</param>
void PrintMatrix(const char* header, int size, double* A)
{
    std::cout << header << std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            std::cout << A[i*size+j] << " ";
        std::cout << std::endl;
    }

}
/// <summary>
/// Вывод на консоль вектора
/// </summary>
/// <param name="header">Заголовок</param>
/// <param name="size">размерность вектора</param>
/// <param name="x">вектор для выыода на консоль</param>
void PrintVector(const char* header, int size, double* x)
{
    std::cout << header << std::endl;
    for (int i = 0; i < size; i++)
        std::cout << x[i] << " ";
    std::cout << std::endl;

}
/// <summary>
/// Вычисление минора матрицы
/// </summary>
/// <param name="n">размерность матрицы</param>
/// <param name="A">матрица минора</param>
/// <param name="i">строка</param>
/// <param name="j">столбец</param>
/// <returns>значение минора</returns>
double Minor(int n, double *A,  int i, int j)
{
    if (i < 0 || j < 0 || i > n - 1 || j > n - 1) return 0.0;
    double* minor = new double[(n - 1) * (n - 1)];

    // заполнение матрицы минора данными
    for (int k = 0; k < n; k++)
    {
        for (int m = 0; m < n; m++)
        {
            if (k < i && m < j)
                minor[k * (n - 1) + m] = A[k * n + m];
            else if(k < i && m > j)
                minor[k * (n - 1) + m-1] = A[k * n + m];
            else if( k > i && m < j)
                minor[(k - 1) * (n - 1) + m] = A[k * n + m];
            else if(k > i && m > j)
                minor[(k -1)*(n-1) + m - 1] = A[k * n + m];
        }
    }

    // вычисление минора по его матрице
    double* b = nullptr, *x = nullptr, det;
    LinearSystemSolve(n - 1, minor, b, x, det, true);
    delete [] minor;
    return det;
}
