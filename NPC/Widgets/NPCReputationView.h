#ifndef NPCREPUTATIONVIEW_H
#define NPCREPUTATIONVIEW_H

#include "NPCCustomWidget.h"

#include <QHash>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;

class NPCReputationView : public NPCCustomWidget
{
    Q_OBJECT
public:
    NPCReputationView(QWidget *parent = nullptr);

    int fame() const;
    QHash<QString, int> reputation() const;

signals:
    void reputationChanged(const int &value);
    void fameChanged(const int &value);

public slots:
    void setPlaceReputation(const QString &place, const int &value);

private slots:
    void updateFame(const int &value);

private:
    NPCCustomWidget *createLayout();

private:
    QLabel *m_pTitle{nullptr};
    QVBoxLayout *m_pLayout{nullptr};
    QHash<QLabel*, QLabel*> m_places;
    int m_fame{0};

    const QString m_titleStyle{
        "QLabel#Title{"
        " padding: 5px;"
        " background-color: black;"
        " color: white;"
        " border-radius: 5px;"
        " font: 20px;"
        "}"
    };

    const QString m_widgetStyle{
        "QWidget#Widget{"
        " border: 1px solid black;"
        " border-radius: 5px;"
        " padding: 5px;"
        "}"
    };
};

#endif // NPCREPUTATIONVIEW_H
