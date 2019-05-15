/****************************************************************************
**
** Copyright (C) 2015 Alexander Rössler
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
#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H

#include <QObject>
#include <QVariant>
#include <QJsonValue>
#include <QTemporaryDir>
#include <memory>

namespace qtquickvcp {

class ApplicationLauncher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant launchers READ launchers NOTIFY launchersChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(QString launchercmdUri READ launchercmdUri WRITE setLaunchercmdUri NOTIFY launchercmdUriChanged)
    Q_PROPERTY(QString launcherUri READ launcherUri WRITE setLauncherUri NOTIFY launcherUriChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)

public:
    explicit ApplicationLauncher(QObject *parent = 0);
    ~ApplicationLauncher();

    QVariant launchers() const;
    bool isSynced() const;

    QString launcherUri() const
    {
        return m_launcherUri;
    }

    bool ready() const
    {
        return m_ready;
    }

    QString launchercmdUri() const
    {
        return m_launchercmdUri;
    }

public slots:
    void start(int index);
    void terminate(int index);
    void kill(int index);
    void writeToStdin(int index, const QString &data);
    void call(const QString &command);
    void shutdown();
    void setImportance(int index, int importance);

    void setLauncherUri(QString launcherUri)
    {
        if (m_launcherUri == launcherUri)
            return;

        m_launcherUri = launcherUri;
        emit launcherUriChanged(m_launcherUri);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready)
            return;

        m_ready = ready;
        emit readyChanged(m_ready);
    }

    void setLaunchercmdUri(QString launchercmdUri)
    {
        if (m_launchercmdUri == launchercmdUri)
            return;

        m_launchercmdUri = launchercmdUri;
        emit launchercmdUriChanged(m_launchercmdUri);
    }

private:
    QObject* m_launcherObject;
    bool m_synced;

    std::unique_ptr<QTemporaryDir> m_temporaryDir;

    void initializeObject();
    void createTemporaryDir();


    QString m_launcherUri;

    bool m_ready;

    QString m_launchercmdUri;

private slots:
    void syncStatus();
    void unsyncStatus();

signals:
    void launchersChanged();
    void syncedChanged(bool arg);
    void connectionStateChanged(int state);
    void launcherUriChanged(QString launcherUri);
    void readyChanged(bool ready);

    void launchercmdUriChanged(QString launchercmdUri);
}; // class ApplicationLauncher
} // namespace qtquickvcp

#endif // APPLICATIONLAUNCHER_H
