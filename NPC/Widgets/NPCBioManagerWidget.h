#ifndef NPCBIOMANAGERWIDGET_H
#define NPCBIOMANAGERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QVector>

class NPCBioManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCBioManagerWidget(QWidget *parent = nullptr);

signals:

public slots:

private:
    QGroupBox *personalBox();
    QGroupBox *formBox();

private:
    QLabel *m_pPortrait{nullptr};
    QLineEdit *m_pName{nullptr};
    QLineEdit *m_pSurname{nullptr};
    QLineEdit *m_pNickname{nullptr};

    QVector<QTextEdit*> m_questions;
};

#endif // NPCBIOMANAGERWIDGET_H
