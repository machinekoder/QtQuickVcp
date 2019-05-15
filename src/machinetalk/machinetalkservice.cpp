#include "machinetalkservice.h"
#include <QDebug>
#include <QQmlComponent>
#include <QQmlPropertyMap>


namespace qtquickvcp {

QString MachinetalkService::enumNameToCamelCase(const QString &name)
{
    QStringList partList;

    partList = name.toLower().split('_');

    for (int i = 0; i < partList.size(); ++i)
    {
        partList[i][0] = partList[i][0].toUpper();
    }

    return partList.join(QLatin1String(""));
}

} // namespace qtquickvcp
