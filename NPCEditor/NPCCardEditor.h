#ifndef NPCCARDEDITOR_H
#define NPCCARDEDITOR_H

#include <QWidget>

#include <QHash>
#include <QJsonDocument>
#include <QVBoxLayout>

#include "Widget/PersonalWidget.h"
#include "Widget/AttributeWidget.h"

class NPCCardEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCardEditor(QWidget *parent = nullptr);
    ~NPCCardEditor();

private:
    QJsonDocument loadJson(const QString &fileName);
    QVBoxLayout *createColumn1();
    QVBoxLayout *createColumn2();
    QVBoxLayout *createColumn3();
    void fillAttributes();

private:
    QJsonDocument m_NPCCard;

    PersonalWidget *m_pPersonal{nullptr};
    QHash<QString, AttributeWidget*> m_attributes;
};

#endif // NPCCARDEDITOR_H
