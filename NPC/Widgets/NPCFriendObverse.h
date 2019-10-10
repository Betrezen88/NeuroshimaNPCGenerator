#ifndef NPCFRIENDOBVERSE_H
#define NPCFRIENDOBVERSE_H

#include "NPCCustomWidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QHash>

class QHBoxLayout;
class QGroupBox;
class QLabel;

class NPCAttributeView;

class NPCFriendObverse : public NPCCustomWidget
{
public:
    explicit NPCFriendObverse(const QJsonObject &personal,
                              const QJsonArray &attributes,
                              const QJsonArray &features,
                              QWidget *parent = nullptr);

    QString fullname() const;
    NPCAttributeView *attribute(const QString &name) const;

private:
    void setAttributes(const QJsonArray &attributes);
    QHBoxLayout *createLayout();
    QGroupBox *personal();
    QGroupBox *features();

private:
    QLabel *m_pName{nullptr};
    QLabel *m_pOrigin{nullptr};
    QLabel *m_pProfession{nullptr};
    QLabel *m_pSickness{nullptr};
    QLabel *m_pConnection{nullptr};

    QHash<QString, NPCAttributeView*> m_attributes;

    QJsonObject m_personal;
    QJsonArray m_features;
};

#endif // NPCFRIENDOBVERSE_H
