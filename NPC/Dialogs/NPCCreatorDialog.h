#ifndef NPCCREATORDIALOG_H
#define NPCCREATORDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QTabWidget>

#include "../Widgets/NPCAttributeManagerWidget.h"
#include "../Widgets/NPCSpecializationManagerWidget.h"
#include "../Widgets/NPCOriginManagerWidget.h"
#include "../Widgets/NPCProfessionManagerWidget.h"
#include "../Widgets/NPCSicknessManagerWidget.h"
#include "../Widgets/NPCSkillsManagerWidget.h"
#include "../Widgets/NPCTrickManagerWidget.h"

#include "../NPCCardTab.h"

#include "../Utils/DataLoader.h"

#include <QJsonArray>

class NPCCreatorDialog : public QDialog
{
public:
    NPCCreatorDialog(QWidget *parent = nullptr);

private:
    QJsonArray m_attributes;
    QTabWidget *m_pTabWidget{nullptr};
    QPushButton *m_pAccept{nullptr};
    QPushButton *m_pClose{nullptr};

    NPCCardTab *m_pCard{nullptr};

    NPCAttributeManagerWidget *m_pAttributeManager{nullptr};
    NPCSpecializationManagerWidget *m_pSpecializationManager{nullptr};
    NPCOriginManagerWidget *m_pOriginManager{nullptr};
    NPCProfessionManagerWidget *m_pProfessionManager{nullptr};
    NPCSicknessManagerWidget *m_pSicknessManager{nullptr};
    NPCSkillsManagerWidget *m_pSkillsManager{nullptr};
    NPCTrickManagerWidget *m_pTricksManager{nullptr};
};

#endif // NPCCREATORDIALOG_H
