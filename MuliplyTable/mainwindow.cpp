#include "mainwindow.h"
#include "protocoldialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    m_multiplyTable->setRowCount(8);
    m_multiplyTable->setColumnCount(8);
    QStringList lst;
    for(int i=2;i<=9;i++)
        lst.append(QString("%1").arg(i));
    m_multiplyTable->setHorizontalHeaderLabels(lst);
    m_multiplyTable->setVerticalHeaderLabels(lst);
    for(int i= 0; i < 8; i++)
        m_multiplyTable->setColumnWidth(i, 10);

    m_errorBackBrush = new QBrush(Qt::GlobalColor::red);
    m_errorForeBrush = new QBrush(Qt::GlobalColor::yellow);

    m_succesBackBrush = new QBrush(Qt::GlobalColor::green);
    m_succesForeBrush = new QBrush(Qt::GlobalColor::blue);

    m_testStarted = false;
    stop_test_action->setEnabled(false);
    result_action->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete m_errorBackBrush;
    delete m_errorForeBrush;

    delete m_succesBackBrush;
    delete m_succesForeBrush;

}

// Заполнить таблицу значениями
void MainWindow::on_populate_table_action_triggered()
{
    m_toFill = true;
    for(int i = 0; i < 8 ; i++)
        for(int j=0; j < 8; j++)
        {
            int res = (i + 2)*(j + 2);
            QTableWidgetItem *wi = new QTableWidgetItem(QString("%1").arg(res));
            wi->setBackground(QBrush(Qt::GlobalColor::white));
            wi->setForeground(QBrush(Qt::GlobalColor::black));
            if(wi->flags() & Qt::ItemIsEditable)
                wi->setFlags(wi->flags() ^ Qt::ItemIsEditable);
            m_multiplyTable->setItem(i,j, wi);
        }
    m_toFill = false;
}


void MainWindow::on_exit_action_triggered()
{
    QApplication::exit();
}

// Очистить таблицу
void MainWindow::on_clear_table_action_triggered()
{    
    for(int i = 0; i < 8 ; i++)
        for(int j=0; j < 8; j++)
        {
            QTableWidgetItem *wi =new QTableWidgetItem(QString(""));
            if(wi->flags() & Qt::ItemIsEditable)
                wi->setFlags(wi->flags() ^ Qt::ItemIsEditable);
            m_multiplyTable->setItem(i,j, wi);
        }
}

// изменено значение в таблице
void MainWindow::on_m_multiplyTable_itemChanged(QTableWidgetItem *item)
{
    if(m_toFill || !m_testStarted) return;
    int row = m_multiplyTable->currentRow() + 2;
    int col = m_multiplyTable->currentColumn() + 2;
    QString value = item->text();
    if(value.isEmpty()) return;
    bool ok;
    int ival = value.toInt(&ok);
    if(!ok)
    {
        QMessageBox::warning(this,"Ошибка","Неверно задано значение");
        return;
    }
    if(row*col != ival)
    {
        item->setBackground(*m_errorBackBrush);
        item->setForeground(*m_errorForeBrush);
        statusBar()->showMessage(QString("%1 x %2 = %3 неправильно!").arg(row).arg(col).arg(ival));
        QString s1 = QString("%1 x %2 = %3 неправильно, ").arg(row).arg(col).arg(ival) +
                QString("%1 x %2 = %3").arg(row).arg(col).arg(row*col);
        if(!m_protocol.contains(s1))
            m_protocol.append(s1);
    }
    else
    {
        item->setBackground(*m_succesBackBrush);
        item->setForeground(*m_succesForeBrush);
        QString s1 = QString("%1 x %2 = %3 правильно!").arg(row).arg(col).arg(ival);
        statusBar()->showMessage(s1);
        if(!m_protocol.contains(s1))
            m_protocol.append(s1);
    }

}

// Начать тест
void MainWindow::on_start_test_action_triggered()
{
    m_testStarted = true;
    start_test_action->setEnabled(false);
    clear_table_action->setEnabled(false);
    populate_table_action->setEnabled(false);
    stop_test_action->setEnabled(true);
    result_action->setEnabled(false);
    on_clear_table_action_triggered();
    if(!m_multiplyTable->isEnabled())
        m_multiplyTable->setEnabled(true);
    setEditableAll(true);
    m_protocol.clear();
}

// Прекратить тест
void MainWindow::on_stop_test_action_triggered()
{
    if(m_testStarted)
    {
        show_results();
    }
    m_testStarted = false;
    start_test_action->setEnabled(true);
    stop_test_action->setEnabled(false);
    clear_table_action->setEnabled(true);
    populate_table_action->setEnabled(true);
    result_action->setEnabled(true);
    setEditableAll(false);

}
// Подвести итоги
void MainWindow::show_results()
{
    int right = 0, not_right = 0;
    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            QTableWidgetItem *item = m_multiplyTable->item(i,j);
            QString txt = item->text();
            if(txt.isEmpty()) continue;
            bool ok;
            int val = txt.toInt(&ok);
            if(!ok) continue;
            if((i + 2)*(j + 2) == val)
                right++;
            else
                not_right++;

        }
    if(not_right == 0)
        QMessageBox::information(this, "Результат", "Поздравляем!\nВы правильно задали все значения");
    else
    {
        int all = right + not_right;
        QMessageBox::information(this, "Результат",
                                 QString("Вы правильно задали %1 значений из %2").arg(right).arg(all));
    }
}
// изменилcz выделенный элемент, если было введено значение - не редактировать
void MainWindow::on_m_multiplyTable_itemSelectionChanged()
{
    QTableWidgetItem *item = m_multiplyTable->currentItem();
    if(!item) return;
    QBrush br = item->background();
    QColor cl = br.color();
    QColor ecl = m_errorBackBrush->color();
    QColor scl = m_succesBackBrush->color();
    if((cl == ecl || cl == scl) && (item->flags() & Qt::ItemIsEditable))
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);

    int row = m_multiplyTable->currentRow() + 2;
    int colum = m_multiplyTable->currentColumn() + 2;
    if(m_testStarted)
        statusBar()->showMessage(QString("%1 x %2 = ?").arg(row).arg(colum));
}

// установить или сбросить флаг редактирования во всех ячейках
void MainWindow::setEditableAll(bool editable)
{
    for(int i = 0; i < 8 ; i++)
        for(int j=0; j < 8; j++)
        {
            QTableWidgetItem *wi = m_multiplyTable->item(i,j);
            if(wi)
            {
                wi->setFlags(editable ? wi->flags() | Qt::ItemIsEditable : wi->flags() ^ Qt::ItemIsEditable);
                m_multiplyTable->setItem(i,j, wi);
            }
        }
}

// Показать результаты теста
void MainWindow::on_result_action_triggered()
{
    QString res = m_protocol.join('\n');
    ProtocolDialog dlg(res, this);
    dlg.exec();
}

