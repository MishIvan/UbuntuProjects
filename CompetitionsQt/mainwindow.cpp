#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    setupUi(this);
    model = new ParticipantsModel();
    participantsView->setModel(model);
    participantsView->setColumnWidth(0, 280);
    connect(participantsView->selectionModel(),
            SIGNAL( currentChanged(const QModelIndex, const QModelInex & ) ),
            this,
            SLOT( test() ) );

}

MainWindow::~MainWindow()
{
    delete model;
}

// добавить участника соревнований
void MainWindow::on_addBunnon_clicked()
{
       QString s1 = age->text();
       int val_age = s1.toInt();
       if( val_age <18 || val_age > 75)
       {
           QMessageBox msg(QMessageBox::Warning,
                                               "Ошибка!",
                                               "Возраст участника должен находиться в интервале от 18 до 75 лет",
                                               QMessageBox::Ok
                                               );
           msg.show();
           return;
       }
       QTime tmb;
       if(val_age >= 18 && val_age <= 30)
           tmb = time1830->time();
       else if(val_age >= 31 && val_age <= 55)
           tmb = time3155->time();
       else
           tmb = time5575->time();
       Participant p;
       p.age = val_age;
       p.name = name->text();
       p.startTime = tmb;
       p.endTime = timeFinish->time();
       model->append(p);
}

void MainWindow::on_delButton_clicked()
{
    QModelIndex idx = participantsView->currentIndex();
    model->deleteRow(idx.row());
}

void MainWindow::on_sortButton_clicked()
{

}


void MainWindow::test()
{
    QModelIndex idx = participantsView->selectionModel()->currentIndex();
    if(idx.isValid())
    {
        if(idx.column() == 0)
            name->setText(model->data(idx, Qt::DisplayRole).toString());
    }

}


void MainWindow::on_action_open_triggered()
{
    
}

void MainWindow::on_action_save_triggered()
{
    
}

void MainWindow::on_action_exit_triggered()
{
    QApplication::exit();
}

