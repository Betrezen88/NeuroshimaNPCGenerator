#ifndef NPCFRIENDVIEW_H
#define NPCFRIENDVIEW_H

#include "NPCCustomWidget.h"

#include <QHash>
#include <QJsonObject>

class QLabel;

class NPCAttributeView;

class NPCFriendView : public NPCCustomWidget
{
public:
    explicit NPCFriendView(const QJsonObject &pal, QWidget *parent = nullptr);

    QString name() const;

private:
    void setName(const QJsonObject &personal);

private:
    QLabel *m_pName{nullptr};
    QLabel *m_pOrigin{nullptr};
    QLabel *m_pProfession{nullptr};
    QLabel *m_pSickness{nullptr};

    QHash<QString, NPCAttributeView*> m_attributes;
};

#endif // NPCFRIENDVIEW_H
