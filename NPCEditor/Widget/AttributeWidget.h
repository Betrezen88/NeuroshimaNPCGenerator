#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>

class AttributeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttributeWidget(const QString &name, const QJsonArray &skillPacks, QWidget *parent = nullptr);

signals:

public slots:

private:
    //AttributeTitleWidget
    //QHash<QString, SkillPackWidget*> m_skillPacks;
};

#endif // ATTRIBUTEWIDGET_H
