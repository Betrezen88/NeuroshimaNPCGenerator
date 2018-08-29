#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QAction>
#include <QMenu>

#include "NPCEditor/NPCCardEditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showAttributeDialog();

private:
    void createActions();
    void createMenus();

private:
    QTabWidget* m_pTabWidget{nullptr};

    QMenu *m_pHeroMenu{nullptr};

    QAction *m_pAttributesAction{nullptr};
};

#endif // MAINWINDOW_H
