#ifndef NPCFRIENDSTATS_H
#define NPCFRIENDSTATS_H

#include <QWidget>
#include <QHash>
#include <QJsonArray>
#include <QLabel>

class QCheckBox;
class QComboBox;
class QGroupBox;
class QLineEdit;
class QPushButton;

class NPCAbstractSkillpackView;
class NPCAttributeView;

class NPCFriendStats : public QWidget
{
    Q_OBJECT
public:
    explicit NPCFriendStats(QWidget *parent = nullptr);
    
    QString name() const;
    QString surname() const;
    QString nickname() const;
    QString profession() const;
    QString origin() const;
    QString sickness() const;
    QString connection() const;
    const QHash<QString, NPCAttributeView*> &attributes() const;
    const QVector<NPCAbstractSkillpackView*> &skillpacks() const;

signals:
    void skillpacksChanged(const QJsonArray &skillpacks, const bool &add);
    void attributesChanged(const QJsonArray &attributes);
    void connectionChanged(const int &value);

public slots:
    void random();

private slots:
    void onArchetypeChanged();
    void onModChanged();
    void setSkillpacksValues(const QJsonArray &skillpacks, const bool &add);

private:
    void init();
    void initConnections(const QJsonArray &connections);
    void initOrigins(const QJsonArray &origins);
    void initArchetypes(const QJsonArray &archetypes);
    void initMods(const QJsonArray &mods);
    QGroupBox *personalBox();
    QGroupBox *attributeBox();

    void randomSkills(const QJsonArray &rules, const QJsonArray &data);
    void randomAttributes();

private:
    QLineEdit *m_pName{nullptr};
    QLineEdit *m_pSurname{nullptr};
    QLineEdit *m_pNickname{nullptr};
    QLineEdit *m_pProfession{nullptr};

    QComboBox *m_pArchetype{nullptr};
    QComboBox *m_pConnection{nullptr};
    QComboBox *m_pOrigin{nullptr};
    QComboBox *m_pSickness{nullptr};
    QComboBox *m_pMod{nullptr};

    QHash<QString, NPCAttributeView*> m_attributes;

    QJsonArray m_attributesValues;
    QJsonArray m_basicSkillpacks;
    QJsonArray m_modSkillpacks;

    QVector<NPCAbstractSkillpackView*> m_skillpacks;
};

#include <QItemDelegate>

class OriginDelegate : public QItemDelegate
{
public:
    explicit OriginDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if ( index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("parent") ) {
            QStyleOptionViewItem parentOption = option;
            parentOption.state |= QStyle::State_Enabled;
            QItemDelegate::paint( painter, parentOption, index );
        }
        else if ( index.data(Qt::AccessibleDescriptionRole).toString() == QLatin1String("child") ) {
            QStyleOptionViewItem childOption = option;
            int indent = option.fontMetrics.horizontalAdvance( QString(4, QChar(' ')) );
            childOption.rect.adjust( indent, 0, 0, 0 );
            childOption.textElideMode = Qt::ElideNone;
            QItemDelegate::paint( painter, childOption, index );
        }
        else {
            QItemDelegate::paint( painter, option, index );
        }
    }
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        return QItemDelegate::sizeHint( option, index );
    }
};

#endif // NPCFRIENDSTATS_H
