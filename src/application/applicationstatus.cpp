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

#include "applicationstatus.h"
#include <QtQml>
#include <machinetalkservice.h>
#include "debughelper.h"

namespace qtquickvcp {

ApplicationStatus::ApplicationStatus(QObject *parent) :
    QObject(parent),
    m_config(new QObject(this)),
    m_motion(new QObject(this)),
    m_io(new QObject(this)),
    m_task(new QObject(this)),
    m_interp(new QObject(this)),
    m_ui(new QObject(this)),
    m_running(false),
    m_synced(false),
    m_syncedChannels(NoChannel),
    m_channels(MotionChannel | ConfigChannel | IoChannel | TaskChannel | InterpChannel),
    m_optionalChannels(UiChannel)
{
    connect(this, &ApplicationStatus::taskChanged,
            this, &ApplicationStatus::updateRunning);
    connect(this, &ApplicationStatus::interpChanged,
            this, &ApplicationStatus::updateRunning);
}

void ApplicationStatus::componentComplete()
{
    initializeObject(MotionChannel);
    initializeObject(ConfigChannel);
    initializeObject(IoChannel);
    initializeObject(TaskChannel);
    initializeObject(InterpChannel);
    initializeObject(UiChannel);
}

void ApplicationStatus::updateSync(ApplicationStatus::StatusChannel channel)
{
    m_syncedChannels |= channel;
    bool flagsMatch = (
                (!(m_channels & MotionChannel) || (m_syncedChannels & MotionChannel)) &&
                (!(m_channels & ConfigChannel) || (m_syncedChannels & ConfigChannel)) &&
                (!(m_channels & IoChannel) || (m_syncedChannels & IoChannel)) &&
                (!(m_channels & TaskChannel) || (m_syncedChannels & TaskChannel)) &&
                (!(m_channels & InterpChannel) || (m_syncedChannels & InterpChannel)) &&
                (!(m_channels & UiChannel) || (m_syncedChannels & UiChannel))
                );
}

void ApplicationStatus::syncStatus()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void ApplicationStatus::unsyncStatus()
{
    m_synced = false;
    m_syncedChannels = 0;
    emit syncedChanged(m_synced);
}

void ApplicationStatus::updateTopics()
{
}


void ApplicationStatus::updateRunning()
{
    bool running;

    running = m_task->property("taskMode").isValid() && m_interp->property("interpState").isValid()
            && ((m_task->property("taskMode").toInt() == TaskModeAuto)
                || (m_task->property("taskMode").toInt() == TaskModeMdi))
            && (m_interp->property("interpState").toInt() != InterpreterIdle);

    if (running != m_running)
    {
        m_running = running;
        emit runningChanged(running);
    }
}

void ApplicationStatus::initializeObject(ApplicationStatus::StatusChannel channel)
{
}
} // namespace qtquickvcp
