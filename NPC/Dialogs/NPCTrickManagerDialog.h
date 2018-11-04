﻿#ifndef NPCTRICKMANAGERDIALOG_H
#define NPCTRICKMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QJsonArray>
#include <QPushButton>

#include "../Widgets/NPCAttributeWidget.h"
#include "../Widgets/NPCTrickWidgetItem.h"

class NPCTrickManagerDialog : public QDialog
{
    Q_OBJECT
public:
    NPCTrickManagerDialog(const QHash<QString, NPCAttributeWidget *> *attributes,
                          QWidget *parent = nullptr);

signals:
    void buyTrick(QListWidgetItem *trick);
    void resignTrick(QListWidgetItem *trick);

public slots:
    void distributeTricks();
    void trickBougth(NPCTrickWidgetItem *trick);
    void trickRemove(NPCTrickWidgetItem *trick);

private:
    void addTricks(const QJsonArray &tricks);
    bool isTrickAvailable(const NPCTrickWidgetItem *pItem) const;

private:
    const QHash<QString, NPCAttributeWidget*> *m_pAttributes;
    QListWidget *m_pAvailable{nullptr};
    QListWidget *m_pUnavailable{nullptr};
    QListWidget *m_pBougth{nullptr};

    QPushButton *m_pAcceptBtn{nullptr};
    QPushButton *m_pCancelBtn{nullptr};
};

#endif // NPCTRICKMANAGERDIALOG_H
