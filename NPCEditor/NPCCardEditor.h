#ifndef NPCCARDEDITOR_H
#define NPCCARDEDITOR_H

#include <QWidget>

#include <QHash>
#include <QJsonDocument>

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
};

#endif // NPCCARDEDITOR_H
