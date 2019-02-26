#ifndef NPCTRICKMANAGERWIDGET_H
#define NPCTRICKMANAGERWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QJsonArray>

#include "../Widgets/NPCAttributeWidget.h"
#include "../Widgets/NPCTrickWidgetItem.h"

class NPCTrickManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCTrickManagerWidget(const QHash<QString, NPCAttributeWidget*> *attributes,
                                   QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void availableTrickDoubleClicked(QListWidgetItem *item);
    void bougthTrickDoubleClicked(QListWidgetItem *item);

private:
    void loadTricks(const QJsonArray &tricks);
    bool isTrickAvailable(const NPCTrickWidgetItem *pItem) const;

private:
    const QHash<QString, NPCAttributeWidget*> *m_pAttributes;
    QListWidget *m_pAvailable{nullptr};
    QListWidget *m_pUnavailable{nullptr};
    QListWidget *m_pBougth{nullptr};
};

#endif // NPCTRICKMANAGERWIDGET_H
