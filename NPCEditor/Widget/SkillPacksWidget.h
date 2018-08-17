#ifndef SKILLPACKSWIDGET_H
#define SKILLPACKSWIDGET_H

#include <QWidget>
#include <QJsonArray>

class SkillPacksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SkillPacksWidget(const QString &name, const QJsonArray &skills, QWidget *parent = nullptr);

signals:

public slots:
};

#endif // SKILLPACKSWIDGET_H
