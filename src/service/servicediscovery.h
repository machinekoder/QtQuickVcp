/****************************************************************************
**
** Copyright (C) 2014 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler @ The Cool Tool GmbH <mail DOT aroessler AT gmail DOT com>
**
****************************************************************************/
#ifndef SERVICEDISCOVERY_H
#define SERVICEDISCOVERY_H

#include <QObject>
#include <QQmlParserStatus>
#include <qjdns.h>
#include <qjdnsshared.h>
#include "servicediscoveryitem.h"
#include "service.h"
#include "servicelist.h"
#include "servicediscoveryfilter.h"
#include "nameserver.h"

namespace qtquickvcp {

class ServiceDiscovery : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool running READ isRunning WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool networkReady READ isNetworkReady NOTIFY networkReadyChanged)
    Q_PROPERTY(bool lookupReady READ isLookupReady NOTIFY lookupReadyChanged)
    Q_PROPERTY(ServiceDiscoveryFilter *filter READ filter CONSTANT)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::ServiceList> serviceLists READ serviceLists)
    Q_PROPERTY(LookupMode lookupMode READ lookupMode WRITE setLookupMode NOTIFY lookupModeChanged)
    Q_PROPERTY(int unicastLookupInterval READ unicastLookupInterval WRITE setUnicastLookupInterval NOTIFY unicastLookupIntervalChanged)
    Q_PROPERTY(int unicastErrorThreshold READ unicastErrorThreshold WRITE setUnicastErrorThreshold NOTIFY unicastErrorThresholdChanged)
    Q_PROPERTY(QQmlListProperty<qtquickvcp::NameServer> nameServers READ nameServers NOTIFY nameServersChanged)
    Q_CLASSINFO("DefaultProperty", "serviceLists")

public:
    enum LookupMode{
        MulticastDNS,
        UnicastDNS
    };
    Q_ENUM(LookupMode)

    explicit ServiceDiscovery(QObject *parent = 0);

    void classBegin() {}
    void componentComplete();

    bool isRunning() const;
    bool isNetworkReady() const;
    ServiceDiscoveryFilter *filter() const;
    bool isLookupReady() const;
    LookupMode lookupMode() const;
    int unicastLookupInterval() const;
    int unicastErrorThreshold() const;

    QQmlListProperty<ServiceList> serviceLists();
    int serviceListCount() const;
    ServiceList *serviceList(int index) const;

    QQmlListProperty<NameServer> nameServers();
    int nameServerCount() const;
    NameServer *nameServer(int index) const;

public slots:
    void setRunning(bool arg);
    void setLookupMode(LookupMode arg);
    void setUnicastLookupInterval(int arg);
    void setUnicastErrorThreshold(int unicastErrorThreshold);
    void updateServices();
    void updateFilter();
    void updateNameServers();
    void addNameServer(NameServer *nameServer);
    void removeNameServer(int index);
    void clearNameServers();

signals:
    void runningChanged(bool arg);
    void networkReadyChanged(bool arg);
    void lookupReadyChanged(bool arg);
    void lookupModeChanged(LookupMode arg);
    void unicastLookupIntervalChanged(int arg);
    void unicastErrorThresholdChanged(int unicastErrorThreshold);
    void nameServersChanged(const QQmlListProperty<NameServer> &arg);

private:
    bool m_componentCompleted;
    bool m_running;
    bool m_networkReady;
    bool m_lookupReady;
    LookupMode m_lookupMode;
    int m_unicastLookupInterval;    // interval for unicast lookups, queries are stop when retriggered
    int m_unicastErrorThreshold;    // amount of unicast lookup timeouts to tolerate
    QPointer<ServiceDiscoveryFilter> m_filter;
    QList<ServiceList*> m_serviceLists;
    QList<NameServer*> m_nameServers;

    QPointer<QTimer> m_networkConfigTimer; // Timer for refreshing the network status

    QPointer<QJDns> m_jdns;
    QMap<int, QJDns::Type> m_queryIdTypeMap; // queryId > type
    QMap<int, ServiceDiscoveryItem *> m_queryIdItemMap; // queryId > item
    QMap<int, QString> m_queryIdServiceMap; // queryId > serviceType
    QMap<QString, QList<ServiceDiscoveryItem*> > m_serviceItemsMap; // serviceType > items
    QMap<QString, QJDns::Type> m_serviceTypeMap; // serviceType > queryType

    QTimer m_unicastLookupTimer;

    void initializeNetworkSession();
    static bool networkConfigIsQualified(const QNetworkConfiguration &config);

    void startAllQueries();
    void stopAllQueries();
    void refreshAllQueries();
    void startQuery(const QString &serviceType);
    void stopQuery(const QString &serviceType);
    void refreshQuery(const QString &serviceType);
    void stopItemQueries(const ServiceDiscoveryItem *item);
    void addServiceType(const QString &serviceType, QJDns::Type queryType);
    void removeServiceType(const QString &serviceType);
    void updateServiceType(const QString &serviceType);
    void removeAllServiceTypes();
    void updateAllServiceTypes();
    static QList<ServiceDiscoveryItem *> filterServiceDiscoveryItems(const QList<ServiceDiscoveryItem *> serviceDiscoveryItems, const ServiceDiscoveryFilter *primaryFilter = nullptr, const ServiceDiscoveryFilter *secondaryFilter = nullptr);
    ServiceDiscoveryItem *addItem(const QString &name, const QString &type);
    ServiceDiscoveryItem *getItem(const QString &name, const QString &type);
    void updateItem(const QString &name, const QString &type);
    void removeItem(const QString &name, const QString &type);
    void clearAlItems(const QString &type);
    void purgeAllItems(const QString &serviceType);

private slots:
    void resultsReady(int id, const QJDns::Response &results);
    void error(int id, QJDns::Error e);
    void openNetworkSession();
    void networkConfigUpdateCompleted();
    bool initializeMdns();
    void deinitializeMdns(bool cleanup);
    void networkSessionOpened();
    void networkSessionClosed();
    void unicastLookup();
}; // class ServiceDiscovery
} // namespace qtquickvcp

#endif // SERVICEDISCOVERY_H
