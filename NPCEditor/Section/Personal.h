#ifndef PERSONAL_H
#define PERSONAL_H

#include "Section.h"

#include <QHash>
#include <QLabel>

class Personal : public Section
{
public:
    Personal(const QJsonArray &json, QWidget *parent = nullptr);
    ~Personal() = default;

private:
    QVBoxLayout *createLayout(const QJsonArray &json);
    QWidget *createWidget(const QString &type);

private:
    QLabel *m_pPortrait{nullptr};
    QLabel *m_pTitle{nullptr};
    QHash<QString, QWidget*> m_widgets;

    const QString m_portraitStyle{ "QLabel{"
                                   " border: 2px solid;"
                                   " border-radius: 5px;"
                                   "}" };
};

#endif // PERSONAL_H
