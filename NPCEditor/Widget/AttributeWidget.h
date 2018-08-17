#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>
#include <QHash>

#include "AttributeTitleWidget.h"
#include "SkillPacksWidget.h"

class AttributeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent = nullptr);
    ~AttributeWidget() = default;

signals:

public slots:

private:
    AttributeTitleWidget *m_pTitle{nullptr};
    QHash<QString, SkillPacksWidget*> m_skillPacks;
};

#endif // ATTRIBUTEWIDGET_H
