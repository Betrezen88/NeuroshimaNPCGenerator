#ifndef NPCHERO_H
#define NPCHERO_H

#include <QObject>
#include <QVector>

#include "NPCAttribute.h"

class NPCHero : public QObject
{
    Q_OBJECT
public:
    explicit NPCHero(QObject *parent = nullptr);

    QString name() const;
    QString origin() const;
    QString profession() const;
    QString specialization() const;
    QString sickness() const;
    QString feature1() const;
    QString feature2() const;
    int reputation() const;
    int fame() const;
    NPCAttribute *attribute(const QString &name) const;
    QVector<NPCAttribute *> attributes() const;
    QVector<QString> tricks() const;

    void setName(const QString &name);
    void setOrigin(const QString &origin);
    void setProfession(const QString &profession);
    void setSpecialization(const QString &specialization);
    void setSickness(const QString &sickness);
    void setFeature1(const QString &feature);
    void setFeature2(const QString &feature);
    void setReputation(const int &reputation);
    void setFame(const int &fame);
    void increaseAttribute(const QString &name, const int &value);
    void setAttributeModificator(const QString &name, const int &value);
    void addTrick(const QString &trick);

private:
    void attribute1();
    void attribute2();
    void attribute3();
    void attribute4();
    void attribute5();

private:
    QString m_name;
    QString m_origin;
    QString m_profession;
    QString m_specialization;
    QString m_sickness;
    QString m_feature1;
    QString m_feature2;
    int m_reputation;
    int m_fame;

    QVector<NPCAttribute*> m_attributes;
    QVector<QString> m_tricks;
};

#endif // NPCHERO_H
