#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDir>
#include <QCoreApplication>
#include <QUuid>
#include <QUrl>
#include <QQmlEngine>

namespace qtquickvcp {

namespace MachinetalkService
{
    QString enumNameToCamelCase(const QString &name);
};
} // namespace qtquickvcp

#endif // SERVICE_H
