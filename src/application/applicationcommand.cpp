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

#include "applicationcommand.h"
#include <QUrl>
#include <QJsonObject>
#include "debughelper.h"

namespace qtquickvcp {

ApplicationCommand::ApplicationCommand(QObject *parent) :
    QObject(parent),
    m_connected(false)
{
}

void ApplicationCommand::abort(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::runProgram(const QString &interpreter, int lineNumber = 0)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::pauseProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::stepProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::resumeProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setSpindleBrake(ApplicationCommand::SpindleBrake brake)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setDebugLevel(int debugLevel)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setFeedOverride(double scale)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setRapidOverride(double scale)
{
    if (!m_connected) {
        return;
    }

}

void ApplicationCommand::setFloodEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::homeAxis(int index)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex)
{
    jog(type, axisIndex, 0.0, 0.0);
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex, double velocity)
{
    jog(type, axisIndex, velocity, 0.0);
}

void ApplicationCommand::jog(ApplicationCommand::JogType type, int axisIndex, double velocity, double distance)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::loadToolTable()
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::updateToolTable(const QJsonArray &toolTable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setMaximumVelocity(double velocity)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::executeMdi(const QString &interpreter, const QString &command)
{
    if (!m_connected) {
        return;
    }

}

void ApplicationCommand::setMistEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setTaskMode(const QString &interpreter, TaskMode mode)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::overrideLimits()
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::openProgram(const QString &interpreter, const QString &filePath)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::resetProgram(const QString &interpreter)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setAdaptiveFeedEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setAnalogOutput(int index, double value)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setBlockDeleteEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setDigitalOutput(int index, bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setFeedHoldEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setFeedOverrideEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setAxisMaxPositionLimit(int axisIndex, double value)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setAxisMinPositionLimit(int axisIndex, double value)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setOptionalStopEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setSpindleOverrideEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setSpindle(ApplicationCommand::SpindleMode mode)
{
    setSpindle(mode, 0.0);
}

void ApplicationCommand::setSpindle(ApplicationCommand::SpindleMode mode, double velocity = 0.0)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setSpindleOverride(double scale)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setTaskState(const QString &interpreter, TaskState state)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setTeleopEnabled(bool enable)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setTeleopVector(double a, double b, double c, double u = 0.0, double v = 0.0, double w = 0.0)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setTrajectoryMode(TrajectoryMode mode)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::unhomeAxis(int index)
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::shutdown()
{
    if (!m_connected) {
        return;
    }
}

void ApplicationCommand::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void ApplicationCommand::clearConnected()
{
    m_connected = false;
    emit connectedChanged(m_connected);
}

} // namespace qtquickvcp
