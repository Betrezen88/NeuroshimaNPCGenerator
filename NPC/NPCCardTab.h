#ifndef NPCCARDTAB_H
#define NPCCARDTAB_H

#include <QTabWidget>
#include <QWidget>

#include "NPCCardObverse.h"
#include "NPCCardReverse.h"

class NPCCardTab : public QTabWidget
{
    Q_OBJECT
public:
    NPCCardTab(QWidget *parent = nullptr);

    NPCCardObverse *obverse() const;
    NPCCardReverse *reverse() const;

private:
    NPCCardObverse *m_pObverse{nullptr};
    NPCCardReverse *m_pReverse{nullptr};

    bool m_editable{true};
};

#endif // NPCCARDTAB_H
