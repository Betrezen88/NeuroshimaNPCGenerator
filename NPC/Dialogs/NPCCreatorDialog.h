#ifndef NPCCREATORDIALOG_H
#define NPCCREATORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>

#include "../Widgets/NPCAttributeManagerWidget.h"

class NPCCreatorDialog : public QDialog
{
public:
    NPCCreatorDialog(QWidget *parent = nullptr);

private:
    QTabWidget *m_pTabWidget{nullptr};
    QPushButton *m_pAccept{nullptr};
    QPushButton *m_pClose{nullptr};

    NPCAttributeManagerWidget *m_pAttributeManager{nullptr};
};

#endif // NPCCREATORDIALOG_H
