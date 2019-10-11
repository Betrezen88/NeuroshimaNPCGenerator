#ifndef NPCCARDFORM_H
#define NPCCARDFORM_H

#include "Widgets/NPCCustomWidget.h"

#include <QJsonArray>
#include <QVector>

class QGridLayout;
class QLabel;
class QPlainTextEdit;

class NPCCardForm : public NPCCustomWidget
{
public:
    explicit NPCCardForm(QWidget *parent = nullptr);

    QVector<QPair<QLabel*, QLabel*>> answers() const;

    void addQuestion(const QString &question, const QString &answer);

    QString notes() const;

private:
    QPlainTextEdit *m_pNotes{nullptr};
    QGridLayout *m_pLayout{nullptr};
    QVector<QPair<QLabel*,QLabel*>> m_answers;

    int m_row{0};
    int m_column{0};
};

#endif // NPCCARDFORM_H
