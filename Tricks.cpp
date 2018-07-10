#include "Tricks.h"

#include <QHBoxLayout>

Tricks::Tricks(const QString &name, const QJsonDocument &json, QWidget *parent)
    : Section(json, parent)
{
    m_tricks = new QListWidget( this );

    QHBoxLayout* all = new QHBoxLayout;
    all->addWidget( create(name) );

    setLayout( all );
}

QGroupBox *Tricks::create(const QString &name)
{
    QGroupBox* groupBox = new QGroupBox( name, this);

    QHBoxLayout* tricksL = new QHBoxLayout;
    tricksL->addWidget( m_tricks );

    groupBox->setLayout( tricksL );

    return groupBox;
}
