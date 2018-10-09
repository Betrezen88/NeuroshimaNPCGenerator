#ifndef DATALOADER_H
#define DATALOADER_H

#include <QJsonArray>

class DataLoader
{
public:
    DataLoader();

    static QJsonArray loadJson(const QString &fileName);
};

#endif // DATALOADER_H
