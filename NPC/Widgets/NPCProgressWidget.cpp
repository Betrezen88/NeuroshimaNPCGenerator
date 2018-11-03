#include "NPCProgressWidget.h"

#include <QGridLayout>

#include <QDebug>

NPCProgressWidget::NPCProgressWidget(QComboBox *pSpecialization, QWidget *parent)
    : QWidget(parent),
      m_pSpecialization(pSpecialization),
      m_pSkillPoints(new QLabel(this)),
      m_pGambels(new QLabel(this)),
      m_pExperience(new QLabel(this))
{
    QGridLayout *pAll = new QGridLayout;
    pAll->addWidget( new QLabel("Umiejętności"), 0, 0 );
    pAll->addWidget( m_pSkillPoints, 0, 1 );
    pAll->addWidget( new QLabel("Gamble"), 1, 0 );
    pAll->addWidget( m_pGambels );
    pAll->addWidget( new QLabel("Punkty Doś."), 2, 0 );
    pAll->addWidget( m_pExperience );

    setLayout( pAll );

    updateLabels();
}

void NPCProgressWidget::onSkillPackBougth(const bool &bougth, const QStringList &specs)
{
    int tSkillPackCost = (bougth) ? 5 : -5;
    if ( specs.contains(m_pSpecialization->currentText()) )
        m_specPoints.second += tSkillPackCost;
    else
        m_skillPoints.second += tSkillPackCost;
    updateLabels();
}

void NPCProgressWidget::onSkillBougth(const int &value, const QStringList &specs, const bool &increase)
{
    int tSkillCost = calculateSkillCost(value, increase);
    if ( specs.contains(m_pSpecialization->currentText()) )
        m_specPoints.second += tSkillCost;
    else
        m_skillPoints.second += tSkillCost;
    updateLabels();
}

void NPCProgressWidget::onRefundPoints(const int value, const QStringList &specs)
{
    if ( specs.contains(m_pSpecialization->currentText()) )
        m_specPoints.second -= value;
    else
        m_skillPoints.second -= value;
    updateLabels();
}

void NPCProgressWidget::onTrickBougth(QListWidgetItem *trick)
{
    if ( m_tricks.second == 0 || (availableExperience() >= m_trickCost) ) {
        if ( m_tricks.second == 0 )
            ++m_tricks.second;
        else
            m_experience.first += m_trickCost;
        emit addTrick( dynamic_cast<NPCTrickWidgetItem*>(trick) );
    }
}

void NPCProgressWidget::updateLabels()
{
    int availableSkillPoints = (m_skillPoints.first + m_specPoints.first)
            - ( m_skillPoints.second + m_specPoints.second );
    int availableGambels = m_gambels.first - m_gambels.second;
    int availableExperience = m_experience.first - m_experience.second;
    m_pSkillPoints->setText( QString::number(availableSkillPoints) );
    m_pGambels->setText( QString::number(availableGambels) );
    m_pExperience->setText( QString::number(availableExperience) );
    emit availableSkillPointsChanged( availableSkillPoints );
}

int NPCProgressWidget::calculateSkillCost(const int &value, const bool &increase)
{
    int cost = 0;
    int multi = (increase) ? 1 : -1;
    int newValue = (increase) ? value : value + 1;

    if ( 1 < newValue )
        cost = newValue * multi;
    else
        cost = 3 * multi;

    return cost;
}

int NPCProgressWidget::availableExperience()
{
    return m_experience.second - m_experience.first;
}
