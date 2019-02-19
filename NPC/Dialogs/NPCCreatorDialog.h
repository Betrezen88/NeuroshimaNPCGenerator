#ifndef NPCCREATORDIALOG_H
#define NPCCREATORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>

#include "../Widgets/NPCAttributeManagerWidget.h"
#include "../Widgets/NPCSpecializationManagerWidget.h"
#include "../Widgets/NPCOriginManagerWidget.h"
#include "../Widgets/NPCProfessionManagerWidget.h"

#include "../NPCCardTab.h"

class NPCCreatorDialog : public QDialog
{
public:
    NPCCreatorDialog(QWidget *parent = nullptr);

private:
    QTabWidget *m_pTabWidget{nullptr};
    QPushButton *m_pAccept{nullptr};
    QPushButton *m_pClose{nullptr};

    NPCAttributeManagerWidget *m_pAttributeManager{nullptr};
    NPCSpecializationManagerWidget *m_pSpecializationManager{nullptr};
    NPCOriginManagerWidget *m_pOriginManager{nullptr};
    NPCProfessionManagerWidget *m_pProfessionManager{nullptr};
    NPCCardTab *m_pCard{nullptr};
};

#endif // NPCCREATORDIALOG_H
