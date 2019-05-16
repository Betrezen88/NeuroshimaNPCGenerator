#include "NPCOtherSkills.h"
#include "NPCSkillChoose.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>

NPCOtherSkills::NPCOtherSkills(QWidget *parent)
    : NPCCustomWidget (parent),
      m_pName(new QLabel("Inne Umiejętności", this)),
      m_pAddBtn(new QPushButton("+", this)),
      m_pSkills(new QListWidget(this))
{
    setObjectName( "OtherSkills" );
    setStyleSheet( m_style );

    m_pName->setObjectName( "Title" );
    m_pName->setStyleSheet( m_title );
    m_pAddBtn->setMaximumWidth( 50 );

    connect( m_pAddBtn, &QPushButton::clicked,
             this, &NPCOtherSkills::showSkillChooseDialog );

    QHBoxLayout *pTitleLayout = new QHBoxLayout;
    pTitleLayout->addWidget( m_pName );
    pTitleLayout->addWidget( m_pAddBtn );

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addLayout( pTitleLayout );
    pLayout->addWidget( m_pSkills );

    setLayout( pLayout );
}

QHash<QLabel *, QPair<QLabel *, SkillSpinBox *> > NPCOtherSkills::skills() const
{
    return m_skills;
}

void NPCOtherSkills::addSkill(const QString name, const QString attribute)
{
    if ( isSkillOnList(name) ) {
        QMessageBox::information(this,
                                 "Zdublowana umiejętność",
                                 "Umiejętność \""+name+"\" została już dodana wcześniej.",
                                 QMessageBox::Ok);
        return;
    }

    QLabel *pName = new QLabel(name);
    QLabel *pAttribute = new QLabel(attribute);
    SkillSpinBox *pSkillBox = new SkillSpinBox(this);
    pSkillBox->setMaximumWidth( 100 );

    connect( pSkillBox, &SkillSpinBox::skillValueChanged,
             [this](const int &value, const bool &increase){
        emit this->skillValueChanged( value, QStringList(), increase );
    } );

    m_skills.insert( pName, QPair<QLabel*, SkillSpinBox*>(pAttribute, pSkillBox) );

    QListWidgetItem *pItem = new QListWidgetItem();

    QWidget *pWidget = new QWidget();
    QPushButton *pRemoveBtn = new QPushButton("X");
    pRemoveBtn->setMaximumWidth( 30 );
    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget( pName );
    pLayout->addWidget( pAttribute );
    pLayout->addWidget( pSkillBox );
    pLayout->addWidget( pRemoveBtn );
    pWidget->setLayout( pLayout );
    pItem->setSizeHint( pWidget->sizeHint() );

    connect( pRemoveBtn, &QPushButton::clicked,
             [this, pItem, pName](){
        emit this->removeSkill(pItem, pName);
    } );

    m_pSkills->addItem( pItem );
    m_pSkills->setItemWidget( pItem, pWidget );
}

void NPCOtherSkills::showSkillChooseDialog()
{
    QDialog *pDialog = new QDialog(this);
    pDialog->setAttribute( Qt::WA_DeleteOnClose );
    NPCSkillChoose *pSkillChoose = new NPCSkillChoose(pDialog);

    connect( pSkillChoose, &NPCSkillChoose::otherSkillChoosed,
             this, &NPCOtherSkills::addSkill );
    connect( pSkillChoose, &NPCSkillChoose::closeWidget,
             pDialog, &QDialog::close );

    pDialog->show();
}

void NPCOtherSkills::removeSkill(QListWidgetItem *row, QLabel *name)
{
    m_skills.remove(name);
    delete m_pSkills->takeItem( m_pSkills->row(row) );
}

bool NPCOtherSkills::isSkillOnList(const QString &name)
{
    for ( QLabel *pName: m_skills.keys() )
        if ( name == pName->text() )
            return true;
    return false;
}

void NPCOtherSkills::onAvailableSkillpointsChanged(const int &skill)
{
    for ( QPair<QLabel*, SkillSpinBox*> pair: m_skills )
        pair.second->onAvailableSkillPointsChanged( skill );
}
