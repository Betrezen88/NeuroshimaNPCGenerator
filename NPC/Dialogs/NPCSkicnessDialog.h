#ifndef NPCSKICNESSDIALOG_H
#define NPCSKICNESSDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QJsonObject>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

class NPCSkicnessDialog : public QDialog
{
    Q_OBJECT
public:
    NPCSkicnessDialog(QWidget *parent = nullptr);

signals:
    void acceptSickness(const QJsonObject &sickness);

private slots:
    void onAccept();
    void randomSicnkess();

private:
    void updateResults(const QJsonArray &results);
    QString createTooltip(const QJsonObject &object);

private:
    QPushButton *m_pRandomBtn{nullptr};
    QPushButton *m_pCancelBtn{nullptr};
    QPushButton *m_pAcceptBtn{nullptr};
    QSpinBox *m_pThrows{nullptr};
    QGroupBox *m_pResultBox{nullptr};

    QVector<QRadioButton*> m_results;
};

#endif // NPCSKICNESSDIALOG_H
