#ifndef NPCATTRIBUTESMODSINFOWIDGET_H
#define NPCATTRIBUTESMODSINFOWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QLabel>

class NPCAttributesModsInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCAttributesModsInfoWidget(const QJsonArray &attributesMods,
                                         QWidget *parent = nullptr);

private:
    QWidget *createInfoWidget(const QJsonArray &attributesMods);

private:
    const QString m_title{"Modyfikatory do cech z posiomów trudności"};
    QLabel *m_pTitleLabel{nullptr};

    // Styles
    const QString m_titleStyle{
        "QLabel#Title{"
        " font: 11px;"
        " color: white;"
        " background-color: black;"
        " padding: 5px;"
        "}"
    };
    const QString m_modWidgetStyle{
        "QWidget#ModWidget{"
        " border: 1px solid;"
        " background-color: gray;"
        " padding: 3px;"
        "}"
    };
    const QString m_modNameStyle{
        "QLabel#ModName{"
        " font: 9px;"
        "}"
    };
    const QString m_modValueStyle{
        "QLabel#ModValue{"
        " border: 1px solid black;"
        " border-radius: 15px;"
        " background-color: white;"
        "}"
    };
};

#endif // NPCATTRIBUTESMODSINFOWIDGET_H
