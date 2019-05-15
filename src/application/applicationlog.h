/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
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
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

#ifndef APPLICATIONLOG_H
#define APPLICATIONLOG_H

#include <QObject>

namespace qtquickvcp {

class ApplicationLogMessage;

class ApplicationLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    Q_PROPERTY(LogLevel logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)
    Q_PROPERTY(QString logUri READ logUri WRITE setLogUri NOTIFY logUriChanged)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)

public:
    ApplicationLog(QObject *parent = nullptr);

    enum LogLevel {
        None,
        Error,
        Warning,
        Info,
        Debug,
        All
    };
    Q_ENUM(LogLevel)

    enum LogOrigin {
        Kernel,
        RtUser,
        Ulapi
    };
    Q_ENUM(LogOrigin)

    bool isConnected() const;
    LogLevel logLevel() const;

    bool ready() const
    {
        return m_ready;
    }

    QString logUri() const
    {
        return m_logUri;
    }

public slots:
    void setLogLevel(LogLevel logLevel);

    void setReady(bool ready)
    {
        if (m_ready == ready)
            return;

        m_ready = ready;
        emit readyChanged(m_ready);
    }

    void setLogUri(QString logUri)
    {
        if (m_logUri == logUri)
            return;

        m_logUri = logUri;
        emit logUriChanged(m_logUri);
    }

signals:
    void connectedChanged(bool connected);
    void logLevelChanged(LogLevel logLevel);
    void messageReceived(const QJsonObject &message);

    void readyChanged(bool ready);

    void logUriChanged(QString logUri);

private:
    bool m_connected;
    LogLevel m_logLevel;

    bool m_ready;

    QString m_logUri;

private slots:
    void updateTopics();
    void setConnected();
    void clearConnected();

}; // class ApplicationLog
} // namespace qtquickvcp

#endif // APPLICATIONLOG_H
