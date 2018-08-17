#ifndef NPCCARDEDITOR_H
#define NPCCARDEDITOR_H

#include <QWidget>

#include <QHash>
#include <QJsonDocument>

#include "Section/Personal.h"
#include "Section/Attributes.h"

class NPCCardEditor : public QWidget
{
    Q_OBJECT
public:
    explicit NPCCardEditor(QWidget *parent = nullptr);
    ~NPCCardEditor();

signals:

public slots:

private:
    QJsonDocument loadJson(const QString &fileName);

private:
    QJsonDocument m_NPCCard;

    Personal *m_pPersonal{nullptr};
    Attributes *m_pAttributes{nullptr};
};

#endif // NPCCARDEDITOR_H
