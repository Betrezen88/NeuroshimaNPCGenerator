#ifndef NPCSPECIALIZATIONMANAGERWIDGET_H
#define NPCSPECIALIZATIONMANAGERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QJsonArray>
#include <QLabel>

class NPCSpecializationManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCSpecializationManagerWidget(QWidget *parent = nullptr);

signals:
    void specializationChanged(const QString &spec);

public slots:


private slots:
    void setSpec(const QString &specName);

private:
    QStringList specs();
    QGroupBox *descriptionBox();

private:
    QComboBox *m_pSpecs{nullptr};
    QLabel *m_pDescriptionLabel{nullptr};

    QJsonArray m_specializations;
};

#endif // NPCSPECIALIZATIONMANAGERWIDGET_H
