#ifndef PROJECTSCARDDIALOG_H
#define PROJECTSCARDDIALOG_H

#include "ui_projectscarddialog.h"

class ProjectsCardDialog : public QDialog, private Ui::ProjectsCardDialog
{
    Q_OBJECT
    bool m_isEditMode;
public:
    explicit ProjectsCardDialog(bool edit_mode = 0,QWidget *parent = nullptr);
private slots:
    void on_ProjectsCardDialog_rejected();
    void on_ProjectsCardDialog_accepted();
};

#endif // PROJECTSCARDDIALOG_H
