#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QAction>
#include <QMenu>

#include "NPC/NPCCard.h"
#include "NPC/NPCCardTab.h"

#include "NPC/Dialogs/NPCAttributeManagerDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAttributeDialog();
    void onAttributeDialogClose();
    void createNewCard();
    void updateTabText(const QString &text);

private:
    void createActions();
    void createMenus();

private:
    QTabWidget* m_pTabWidget{nullptr};
    QVector<NPCCardTab*> m_cards;
    NPCAttributeManagerDialog *m_pAttributeDialog{nullptr};

    QMenu *m_pHeroMenu{nullptr};

    QAction *m_pAttributesAction{nullptr};
    QAction *m_pNewCardAction{nullptr};
};

#endif // MAINWINDOW_H
