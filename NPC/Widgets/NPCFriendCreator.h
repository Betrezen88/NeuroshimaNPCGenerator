#ifndef NPCFRIENDCREATOR_H
#define NPCFRIENDCREATOR_H

#include <QJsonArray>
#include <QJsonObject>
#include <QTabWidget>
#include <QWidget>
#include <QVector>

class QComboBox;
class QDialog;
class QLabel;
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QPlainTextEdit;
class QPushButton;
class QScrollArea;
class QSpinBox;

class NPCFeatureWidget;

class NPCFriendCreator : public QWidget
{
    Q_OBJECT
public:
    explicit NPCFriendCreator(QString cash, QWidget *parent = nullptr);

signals:
    void connectionValueChanged(const int &value);
    void connectionCostChanged(const int &value);
    void featureCostChanged(const int &value);
    void profitCostChanged(const int &value);

public slots:

private slots:
    void onConnectionValueChange(const int &value);
    void onAvailableFeatureClick(QListWidgetItem *pItem);
    void onAddBtnClick();
    void buyFeature(const QJsonObject &feature);
    void setCost(const QString &type, const int &value);
    void checkFeatureAvailability();

private:
    void init();
    QWidget *createPersonalTab();
    QWidget *createFeaturesTab();
    QJsonArray jsonData(const QString &category);
    QDialog *createDialog(const QJsonObject &feature);
    NPCFeatureWidget *createWidget(const QJsonObject &feature);

private:
    QString m_cash;

    int m_connectionCost{0};
    int m_featuresCost{0};
    int m_profit{0};

    QLabel *m_pAvailableCash{nullptr};
    QLabel *m_pSpendedCash{nullptr};

    QTabWidget *m_pTabWidget{nullptr};
    QLineEdit *m_pName{nullptr};
    QLineEdit *m_pSurname{nullptr};
    QLineEdit *m_pNickname{nullptr};
    QLineEdit *m_pProfession{nullptr};

    QComboBox *m_pConnection{nullptr};

    QListWidget *m_pAvailableFeatures{nullptr};
    QListWidget *m_pFeatures{nullptr};

    QPushButton *m_pRemoveBtn{nullptr};
    QPushButton *m_pAddBtn{nullptr};

    QPushButton *m_pOkBtn{nullptr};
    QPushButton *m_pCloseBtn{nullptr};

    QVector<QComboBox*> m_selections;

    QVector<QPlainTextEdit*> m_answers;
    QJsonArray m_connections;
};

#endif // NPCFRIENDCREATOR_H
