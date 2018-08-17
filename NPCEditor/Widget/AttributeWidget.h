#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>

#include "AttributeTitleWidget.h"

class AttributeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent = nullptr);

signals:

public slots:

private:
    AttributeTitleWidget *m_pTitle{nullptr};
    //QHash<QString, SkillPackWidget*> m_skillPacks;
};

#endif // ATTRIBUTEWIDGET_H
