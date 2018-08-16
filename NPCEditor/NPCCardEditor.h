#ifndef NPCCARDEDITOR_H
#define NPCCARDEDITOR_H

#include <QWidget>

#include <QHash>
#include <QJsonDocument>

#include "Section/Personal.h"

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
    QHash<QString, QJsonDocument> m_json;

    Personal *m_pPersonal{nullptr};
};

#endif // NPCCARDEDITOR_H
