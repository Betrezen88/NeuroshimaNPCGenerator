#include "NPCBioManagerWidget.h"

#include "../Utils/DataLoader.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

NPCBioManagerWidget::NPCBioManagerWidget(QWidget *parent)
    : QWidget(parent),
      m_pPortrait(new QLabel("Portret",this)),
      m_pName(new QLineEdit(this)),
      m_pSurname(new QLineEdit(this)),
      m_pNickname(new QLineEdit(this))
{
    QPixmap portait( ":/images/icons/Hero_portrait_icon.png" );
    portait.scaled( 140, 200, Qt::KeepAspectRatio );

    m_pPortrait->setMaximumSize( 140, 200 );
    m_pPortrait->setPixmap( portait );
    QGridLayout *pLayout = new QGridLayout;
    setLayout( pLayout );

    m_pName->setPlaceholderText( "Imię" );
    m_pSurname->setPlaceholderText( "Nazwisko" );
    m_pNickname->setPlaceholderText( "Ksywa" );

    connect( m_pName, &QLineEdit::textChanged,
             this, &NPCBioManagerWidget::setHeroName );
    connect( m_pNickname, &QLineEdit::textChanged,
             this, &NPCBioManagerWidget::setHeroName );
    connect( m_pSurname, &QLineEdit::textChanged,
             this, &NPCBioManagerWidget::setHeroName );

    pLayout->addWidget( personalBox(), 0, 0, 1, 4 );
    pLayout->addWidget( formBox(), 1, 0, 1, 4 );
}

void NPCBioManagerWidget::mousePressEvent(QMouseEvent *event)
{
    if ( childAt(event->pos()) == m_pPortrait ) {
        QString fileName = QFileDialog::getOpenFileName( this,
                                                         "Wybierz portret",
                                                         QDir::homePath(),
                                                         "*.img, *.png, *.jpg" );
        if ( !fileName.isEmpty() ) {
            QPixmap portrait(fileName);
            portrait.scaled( 140, 200, Qt::KeepAspectRatio );
            m_pPortrait->setScaledContents( true );
            m_pPortrait->setPixmap( portrait );
            portraitChanged( portrait );
        }
    }
}

void NPCBioManagerWidget::setHeroName()
{
    QString name = m_pName->text();
    if ( !m_pNickname->text().isEmpty() )
        name = name + " '" + m_pNickname->text()+"'";
    if ( !m_pSurname->text().isEmpty() )
        name = name + " " +m_pSurname->text();
    emit nameChanged( name );
}

QGroupBox *NPCBioManagerWidget::personalBox()
{
    QGroupBox *pPersonalBox = new QGroupBox( "Dane osobowe", this );
    QHBoxLayout *pLayout = new QHBoxLayout;

    QVBoxLayout *pDataLayout = new QVBoxLayout;
    pDataLayout->addWidget( m_pName );
    pDataLayout->addWidget( m_pSurname );
    pDataLayout->addWidget( m_pNickname );

    pLayout->addWidget( m_pPortrait );
    pLayout->addLayout( pDataLayout );

    pPersonalBox->setLayout( pLayout );
    return pPersonalBox;
}

QGroupBox *NPCBioManagerWidget::formBox()
{
    QGroupBox *pFormmBox = new QGroupBox( "Formularz", this );
    QHBoxLayout *pLayout = new QHBoxLayout;
    QVBoxLayout *pColumn1 = new QVBoxLayout;
    QVBoxLayout *pColumn2 = new QVBoxLayout;

    const QJsonArray &questions = DataLoader::loadJson( ":/data/json/Questions.json" );

    for ( const QJsonValue tQuestion: questions ) {
        const QJsonObject &question = tQuestion.toObject();
        QVBoxLayout *pColumn = (question.value("no").toInt() <= 3) ? pColumn1: pColumn2;
        QLabel *pQuestion = new QLabel(question.value("question").toString());
        pQuestion->setWordWrap( true );
        pColumn->addWidget( pQuestion );
        QLabel *pDescription = new QLabel(question.value("description").toString());
        pDescription->setWordWrap( true );
        pColumn->addWidget( pDescription );
        m_questions.push_back( new QTextEdit(this) );
        pColumn->addWidget( m_questions.last() );
    }
    pLayout->addLayout( pColumn1 );
    pLayout->addLayout( pColumn2 );
    pFormmBox->setLayout( pLayout );
    return pFormmBox;
}
