#include "NPCCardForm.h"

#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPlainTextEdit>

NPCCardForm::NPCCardForm(QWidget *parent)
    : NPCCustomWidget( parent ),
      m_pNotes( new QPlainTextEdit(this) ),
      m_pLayout( new QGridLayout )
{
    setLayout( m_pLayout );

    QVBoxLayout *pNotesL = new QVBoxLayout;
    QLabel *pLabel = new QLabel("Notatki");
    pLabel->setAlignment( Qt::AlignHCenter );
    pNotesL->addWidget( pLabel );
    pNotesL->addWidget( m_pNotes );

    m_pLayout->addLayout( pNotesL, 0, 2, 4, 2 );
}

QVector<QPair<QLabel *, QLabel *> > NPCCardForm::answers() const
{
    return m_answers;
}

void NPCCardForm::addQuestion(const QString &question, const QString &answer)
{
    const QString &num = QString::number( m_answers.count()+1 );
    QLabel *pQuestion = new QLabel( num+") "+question );
    pQuestion->setWordWrap( true );
    pQuestion->setStyleSheet( "font-weight: bold;" );
    QLabel *pAnswer = new QLabel( answer );
    pAnswer->setWordWrap( true );
    m_answers.push_back( QPair<QLabel*, QLabel*>(pQuestion, pAnswer) );

    QVBoxLayout *pQuestionL = new QVBoxLayout;
    pQuestionL->addWidget( pQuestion, 1 );
    pQuestionL->addWidget( pAnswer, 1, Qt::AlignTop );

    m_pLayout->addLayout( pQuestionL, m_row, m_column );

    m_row = (m_answers.count()%2 == 0) ? m_row+1 : m_row;
    m_column = (m_column == 0) ? m_column+1 : 0;
}

QString NPCCardForm::notes() const
{
    return m_pNotes->toPlainText();
}
