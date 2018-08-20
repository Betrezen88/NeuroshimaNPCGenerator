#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include "CardWidget.h"
#include "AttributeTitleWidget.h"
#include "SkillPacksWidget.h"

#include <QHash>

class AttributeWidget : public CardWidget
{
    Q_OBJECT
public:
    explicit AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent = nullptr);
    ~AttributeWidget() = default;

private:
    AttributeTitleWidget *m_pTitleBar{nullptr};
    QHash<QString, SkillPacksWidget*> m_skillPacks;
};

#endif // ATTRIBUTEWIDGET_H
