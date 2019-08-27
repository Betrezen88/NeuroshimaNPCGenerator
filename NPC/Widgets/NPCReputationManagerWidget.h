#ifndef NPCREPUTATIONMANAGERWIDGET_H
#define NPCREPUTATIONMANAGERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHash>
#include <QGridLayout>
#include <QGroupBox>

#include "ReputationValueBox.h"

class NPCReputationManagerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NPCReputationManagerWidget(QWidget *parent = nullptr);

    QHash<QString, int> reputations() const;
    int fame() const;

signals:
    void availabeReputationPointsChanged(const int &value);
    void freeReputationPointsChanged(const int &value);

public slots:
    void setPlaceReputation(const QString &name);

private:
    void checkFame();

private:
    QGroupBox *reputationBox();
    QGroupBox *fameBox();
    QGroupBox *pointsBox();
    QGroupBox *descriptionBox();

private:
    QGridLayout *m_pLayout{nullptr};
    QLabel *m_pFame{nullptr};
    QLabel *m_pPoints{nullptr};

    QHash<QString, ReputationValueBox*> m_reputation;

    const QString m_reputationDesc{ "Testy reputacji mozna wykonać za każdy razem gdy "
                                    "BG spotyka postać niezależną. Wtedy MG określa "
                                    "poziom trudności rzutu 3k20. Zdany test oznacza,"
                                    " że BG słyszał o postaci i traktuje ją odpowiednio"
                                    " lepiej. MG może zadecydować, że zdany test reputacji"
                                    " ma odzwierciedlenie w testowaniu niektórych "
                                    "umiejętności(np. Zastraszania, Perswazji lub Blefu)."
                                    " W każdym terenie można zdobyć maksymalnie"
                                    " 20 punktów reputacji. " };

    const QString m_fameDesc{ "Punkt sławy zyskuje się po zdobyciu 20 punktów reputacji"
                              " w danym miejcu. Każdy punkt sławy traktowany jest jako "
                              "dodatkowy punkt reputacji działający w każdym zakątku "
                              "Stanów. W testach reputacji sława działa jak umiejętność,"
                              " pozwala zmniejszyć wynik na kościach, również działa "
                              " zasada suwaka, czyli obniży poziom trudności testu." };
};

#endif // NPCREPUTATIONMANAGERWIDGET_H
