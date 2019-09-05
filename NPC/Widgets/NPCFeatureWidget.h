#ifndef NPCFEATUREWIDGET_H
#define NPCFEATUREWIDGET_H

#include <QWidget>
#include <QJsonObject>

class QComboBox;
class QLabel;

class NPCFeatureWidget : public QWidget
{
    Q_OBJECT
public:
    NPCFeatureWidget(const QJsonObject &feature, QWidget *parent = nullptr);
    virtual ~NPCFeatureWidget() = default;

signals:
    void sendFeature(const QJsonObject &feature);

public slots:
    virtual void createFeature() = 0;

protected:
    QJsonObject m_feature;
};

class SkillSelector : public NPCFeatureWidget
{
    Q_OBJECT
public:
    explicit SkillSelector(const QJsonObject &feature, QWidget *parent = nullptr);
    ~SkillSelector() = default;

public slots:
    void createFeature();

private:
    QStringList skills() const;

private:
    QComboBox *m_pSkill{nullptr};
};

class SkillsSelector : public NPCFeatureWidget
{
    Q_OBJECT
public:
    explicit SkillsSelector(const QJsonObject &feature, QWidget *parent = nullptr);
    ~SkillsSelector() = default;

public slots:
    void createFeature();

private slots:
    void onSpecializationChanged(const QString &spec);
    void setUniqueSkills(QComboBox *pSkill, const QString &text);

private:
    QStringList specializations() const;
    QStringList skillsBySpec(const QString &spec) const;

private:
    QComboBox *m_pSpec{nullptr};
    QVector<QComboBox*> m_skills;
};

class AttributeSelector : public NPCFeatureWidget
{
    Q_OBJECT
public:
    AttributeSelector(const QJsonObject &feature, QWidget *parent = nullptr);
    ~AttributeSelector() = default;

public slots:
    void createFeature();

private:
    QStringList attributes() const;

private:
    QComboBox *m_pAttribute{nullptr};
};

class ReputationSelector : public NPCFeatureWidget
{
    Q_OBJECT
public:
    ReputationSelector(const QJsonObject &feature, QWidget *parent = nullptr);
    ~ReputationSelector() = default;

public slots:
    void createFeature();

private:
    QStringList places() const;

private:
    QComboBox *m_pPlace{nullptr};
};

class DebtSelector : public NPCFeatureWidget
{
    Q_OBJECT
public:
    explicit DebtSelector(const QJsonObject &feature, QWidget *parent = nullptr);

public slots:
    void createFeature();

private slots:
    void setProfitValue(const int &index);

private:
    QComboBox *m_pDebt{nullptr};
    QLabel *m_pProfit{nullptr};
};

#endif // NPCFEATUREWIDGET_H
