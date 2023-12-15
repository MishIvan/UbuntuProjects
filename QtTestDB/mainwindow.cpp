#include "mainwindow.h"
#include "recordsdialog.h"
#include <QMessageBox>
#include <QSqlQuery>

extern QSqlDatabase m_database;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    // инициализация модели данных
    m_model = new QSqlTableModel(nullptr, m_database);

    m_model->setTable("test_table");
    m_model->select();

    m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Наименование"));
    m_model->setHeaderData(2, Qt::Horizontal, QObject::tr("Переключатели"));
    m_model->setHeaderData(3, Qt::Horizontal, QObject::tr("Целое\nчисло"));
    m_model->setHeaderData(4, Qt::Horizontal, QObject::tr("Вещественное\nчисло"));

    // ассоциировать модель даных с видом
    m_tableView->setModel(m_model);
    m_tableView->setColumnHidden(0, true);
    m_tableView->setColumnWidth(1, 150);
    m_tableView->resizeColumnsToContents();
}

MainWindow::~MainWindow()
{
    delete m_model;
    if(m_database.isOpen())
        m_database.close();
}

/// завершение работы с програмой
void MainWindow::on_exit_action_triggered()
{
    QApplication::exit();
}

/// добавление записи
void MainWindow::on_add_rocord_action_triggered()
{
    RecordsDialog dlg(m_model,QModelIndex(),this);
    dlg.setWindowTitle(tr("Добавление новой записи записи"));
    dlg.setModal(true);
    if(dlg.exec() == QDialog::Accepted)
        statusBar()->showMessage(tr("Добавлена новая запись"));

}

/// изменение текущей записи
void MainWindow::on_edit_record_action_triggered()
{
    QItemSelectionModel *selmodel =  m_tableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    if (idx.isValid())
    {
        RecordsDialog dlg(m_model,idx,this);
        dlg.setWindowTitle(tr("Изменение записи"));
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
            statusBar()->showMessage(tr("Текущая запись изменена"));
    }

}

/// удаление текущей записи
void MainWindow::on_delete_record_action_triggered()
{
    QItemSelectionModel *selmodel =  m_tableView->selectionModel();
    QModelIndex idx = selmodel->currentIndex();
    if (idx.isValid())
    {
        if(QMessageBox::question(this,tr("Предупреждение"),
                                 tr("Вы действительно хотите удалить текущую запись"),
                                 QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            long id = m_model->data( m_model->index(idx.row(),0) ).toLongLong();
            QSqlQuery qr(m_database);
            QString qText = QString("delete from test_table where id = %1").arg(id);
            if(qr.exec(qText))
            {
                m_model->removeRow(idx.row());
                m_model->submitAll();
                m_model->select();
                statusBar()->showMessage(tr("Текущая запись удалена"));
            }
        }
    }

}

