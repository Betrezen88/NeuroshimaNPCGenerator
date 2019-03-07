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
    explicit NPCTrickManagerWidget(const QHash<const QString, NPCAttributeWidget*> *attributes,
                                   QWidget *parent = nullptr);

signals:

public slots:
    void setBonusTrick(const QString &name);
    void removeBonusTrick(const QString &name);
    void loadTricks();

private slots:
    void availableTrickDoubleClicked(QListWidgetItem *item);
    void bougthTrickDoubleClicked(QListWidgetItem *item);

private:
    bool isTrickAvailable(const NPCTrickWidgetItem *pItem) const;

private:
    const QHash<const QString, NPCAttributeWidget*> *m_pAttributes;
    QListWidget *m_pAvailable{nullptr};
    QListWidget *m_pUnavailable{nullptr};
    QListWidget *m_pBougth{nullptr};

    int m_freeTricks{0};
};

#endif // NPCTRICKMANAGERWIDGET_H
