#include "NPCWeaponItem.h"
#include "../Utils/DataLoader.h"

#include <QFrame>
#include <QLabel>
#include <QJsonArray>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

NPCWeaponItem::NPCWeaponItem(const QJsonObject &item, const int &body, QWidget *parent)
    : NPCCustomWidget(parent),
      m_item(item),
      m_body(body),
      m_pLayout(new QVBoxLayout),
      m_pName(new QLabel(this)),
      m_pLine(new QFrame(this))
{
    setLayout( m_pLayout );
    m_pLayout->setSpacing( 2 );
    m_pName->setAlignment( Qt::AlignCenter );
    m_pName->setText("<b>"+m_item.value("name").toString()+"</b>");
    m_pLine->setFrameShape( QFrame::HLine );
    m_pLine->setFrameShadow( QFrame::Sunken );

    m_pLayout->addWidget( m_pName );

    if ( "Broń biała" == m_item.value("type").toString() )
        handWeaponLayout();
    else if ( "Broń miotana" == m_item.value("type").toString()
              || "Pistolet" == m_item.value("type").toString()
              || "Rewolwer" == m_item.value("type").toString()
              || "Pistolet maszynowy" == m_item.value("type").toString()
              || "Karabin powtarzalny" == m_item.value("type").toString()
              || "Karabin samopowtarzalny" == m_item.value("type").toString()
              || "Karabin automatyczny" == m_item.value("type").toString()
              || "Strzelba" == m_item.value("type").toString()
              || "Karabin maszynowy" == m_item.value("type").toString()
              || "Karabin snajperski" == m_item.value("type").toString()
              || "Granatnik" == m_item.value("type").toString()
              || "Broń przeciwpancerna" == m_item.value("type").toString()
              || "Miotacz ognia" == m_item.value("type").toString() )
        gunLayout();

    m_pLayout->addWidget( m_pLine );
}

void NPCWeaponItem::gunLayout()
{
    QHBoxLayout *pDamageRow = new QHBoxLayout;
    m_pLayout->addLayout( pDamageRow );
    pDamageRow->addWidget( addDamage() );

    if ( m_item.contains("penetration") )
        pDamageRow->addWidget( addPenetration() );
    else
        pDamageRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum) );

    if ( m_item.contains("ammo") && m_item.contains("magazine") ) {
        QHBoxLayout *pAmmoRow = new QHBoxLayout;
        pAmmoRow->addWidget( addAmmo() );
        pAmmoRow->addWidget( addMagazine() );
        m_pLayout->addLayout( pAmmoRow );
    }

    if ( "Broń miotana" != m_item.value("type").toString() ) {
        QHBoxLayout *pJamRow = new QHBoxLayout;
        pJamRow->addWidget( addJam() );
        if ( m_item.contains("firerate") )
            pJamRow->addWidget( addFirerate() );
        else
            pJamRow->addSpacerItem( new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        m_pLayout->addLayout( pJamRow );
    }

    if ( m_item.contains("special") )
        m_pLayout->addWidget( addSpecial() );
}

void NPCWeaponItem::handWeaponLayout()
{
    if ( m_item.contains("bonus") )
        m_pLayout->addWidget( addDexterityBonus() );

    m_pLayout->addWidget( addDamage() );

    if ( m_item.contains("penetration") )
        addPenetration();

    if ( m_item.contains("special") )
        addSpecial();
}

QLabel *NPCWeaponItem::addDexterityBonus()
{
    QLabel *pBonus = new QLabel(this);
    pBonus->setMinimumHeight(20);
    QString bonuses;
    for ( const QJsonValue bonus: m_item.value("bonus").toArray() ) {
        bonuses += bonus.toString();
        if ( bonus != m_item.value("bonus").toArray().last() )
            bonuses += " / ";
    }
    pBonus->setText( "<b>Bonus do Zręczności: </b><i>"+bonuses+"</i>" );
    return pBonus;
}

QLabel *NPCWeaponItem::addDamage()
{
    QLabel *pDamage = new QLabel(this);
    pDamage->setMinimumHeight(20);
    QString damageValue;
    if ( m_item.value("damage").isArray() ) {
        for ( const QJsonValue dmg: m_item.value("damage").toArray() ) {
            QRegExp value = QRegExp("(\\d+)");
            value.indexIn(dmg.toString());
            if ( m_body <= value.cap(1).toInt() ) {
                value.setPattern("(\\b.\\/.\\/.\\b)");
                value.indexIn(dmg.toString());
                damageValue = damage(value.cap(1));
                break;
            }
        }
    }
    else {
        QString damageText = m_item.value("damage").toString();
        if ( 1 < damageText.count() )
            damageValue = damage(damageText.remove("(").remove(")"));
        else
            damageValue = damageText;
    }
    pDamage->setText( "<b>Obrażenia: </b><i>"+damageValue+"</i>" );
    return pDamage;
}

QLabel *NPCWeaponItem::addSpecial()
{
    QJsonArray array = DataLoader::loadJson(":/data/json/WeaponSpecials.json");
    QLabel *pSpecial = new QLabel(this);
    pSpecial->setMinimumHeight(20);
    QString tooltip = "<div style=\"width: 450px;\">";
    QString specials;
    for ( QJsonValue special: m_item.value("special").toArray() ) {
        specials += special.toString();
        for ( const QJsonValue t: array ) {
            QJsonObject obj = t.toObject();
            if ( special.toString() == obj.value("name").toString() )
                tooltip += "<b>"+obj.value("name").toString() + "</b> - "
                        + obj.value("description").toString();
        }
        if ( special != m_item.value("special").toArray().last() ) {
            specials += ", ";
            tooltip += "<br><br>";
        }
    }
    tooltip += "</div>";
    pSpecial->setText( "<b>Specialne: </b><i>"+specials+"</i>" );
    pSpecial->setToolTip( tooltip );
    pSpecial->setToolTipDuration( 5*60*100 );
    return pSpecial;
}

QLabel *NPCWeaponItem::addPenetration()
{
    QLabel *pPenetration = new QLabel(this);
    pPenetration->setMinimumHeight(20);
    pPenetration->setText( "<b>Przebicie: </b><i>"
                            +QString::number(m_item.value("penetration").toInt())+"</i>" );
    return pPenetration;
}

QLabel *NPCWeaponItem::addAmmo()
{
    QLabel *pAmmo = new QLabel(this);
    pAmmo->setMinimumHeight(20);
    pAmmo->setText("<b>Nabój: </b><i>"+m_item.value("ammo").toString()+"</i>");
    return pAmmo;
}

QLabel *NPCWeaponItem::addMagazine()
{
    QLabel *pMagazine = new QLabel(this);
    pMagazine->setMinimumHeight(20);
    pMagazine->setText("<b>Mazgazynek: </b><i>"+m_item.value("magazine").toString()+"</i>");
    return pMagazine;
}

QLabel *NPCWeaponItem::addJam()
{
    QLabel *pJam = new QLabel(this);
    pJam->setMinimumHeight(20);
    pJam->setText("<b>Zacięcie: </b><i>17-20</i>");
    return pJam;
}

QLabel *NPCWeaponItem::addFirerate()
{
    QLabel *pFirerate = new QLabel(this);
    pFirerate->setMinimumHeight(20);
    pFirerate->setText("<b>Szybkostrzelność: </b><i>"
                       +QString::number(m_item.value("firerate").toInt())+"</i>");
    return pFirerate;
}

QString NPCWeaponItem::damage(QString text)
{
    QString dmgText;
    int index{0};
    for ( int i{0}; i<text.count(); ++i )
        if ( text.at(i).isLetter() ) {
            dmgText += QString::number(++index) + "s " +text.at(i);
            if ( i != text.count()-1 )
                dmgText += " / ";
        }
    return dmgText;
}
