#include "mainwindow.h"

#include <QApplication>

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

int main(int argc, char *argv[])
{
    char buff[1024];
    GetFullPathInWD(argv[0],"TasksAccounting.db",buff);
    QString path = buff;
    QApplication a(argc, argv);
    MainWindow w(path);
    w.show();
    return a.exec();
}
