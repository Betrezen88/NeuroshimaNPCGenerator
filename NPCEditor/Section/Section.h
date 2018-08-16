#ifndef SECTION_H
#define SECTION_H

#include <QWidget>

class Section : public QWidget
{
    Q_OBJECT
public:
    explicit Section(QWidget *parent = nullptr): QWidget(parent) {}

signals:

public slots:

protected:
    const QString m_titleStyle{};
};

#endif // SECTION_H
