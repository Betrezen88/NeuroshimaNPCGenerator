#ifndef NPCBIOMANAGERWIDGET_H
#define NPCBIOMANAGERWIDGET_H

#include <QWidget>
#include <QMouseEvent>
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
    void nameChanged(const QString name);

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void setHeroName();

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
