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

#ifndef APPLICATIONCOMMAND_H
#define APPLICATIONCOMMAND_H

#include <QObject>
#include <QJsonArray>

namespace qtquickvcp {

class ApplicationCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:
    explicit ApplicationCommand(QObject *parent = 0);

    enum SpindleBrake {
        ReleaseBrake,
        EngageBrake
    };
    Q_ENUM(SpindleBrake)

    enum JogType {
        StopJog = 0,
        ContinuousJog = 1,
        IncrementJog = 2
    };
    Q_ENUM(JogType)

    enum SpindleMode {
        SpindleForward,
        SpindleReverse,
        SpindleOff,
        SpindleDecrease,
        SpindleIncrease,
        SpindleConstant
    };
    Q_ENUM(SpindleMode)

    enum TaskState {
        TaskStateEstop,
        TaskStateEstopReset,
        TaskStateOff,
        TaskStateOn
    };
    Q_ENUM(TaskState)

    enum TaskMode {
        TaskModeManual,
        TaskModeAuto,
        TaskModeMdi
    };
    Q_ENUM(TaskMode)

    enum TrajectoryMode {
        FreeMode,
        CoordinatedMode,
        TeleopMode
    };
    Q_ENUM(TrajectoryMode)

    bool isConnected() const
    {
        return m_connected;
    }

public slots:
    void abort(const QString &interpreter);
    void runProgram(const QString &interpreter, int lineNumber);
    void pauseProgram(const QString &interpreter);
    void stepProgram(const QString &interpreter);
    void resumeProgram(const QString &interpreter);
    void resetProgram(const QString &interpreter);
    void setTaskMode(const QString &interpreter, TaskMode mode);
    void setTaskState(const QString &interpreter, TaskState state);
    void openProgram(const QString &interpreter, const QString &fileName);
    void executeMdi(const QString &interpreter, const QString &command);
    void setSpindleBrake(SpindleBrake brake);
    void setDebugLevel(int debugLevel);
    void setFeedOverride(double scale);
    void setRapidOverride(double scale);
    void setFloodEnabled(bool enable);
    void homeAxis(int index);
    void jog(JogType type, int axisIndex);
    void jog(JogType type, int axisIndex, double velocity);
    void jog(JogType type, int axisIndex, double velocity, double distance);
    void loadToolTable();
    void updateToolTable(const QJsonArray &toolTable);
    void setMaximumVelocity(double velocity);
    void setMistEnabled(bool enable);
    void overrideLimits();
    void setAdaptiveFeedEnabled(bool enable);
    void setAnalogOutput(int index, double value);
    void setBlockDeleteEnabled(bool enable);
    void setDigitalOutput(int index, bool enable);
    void setFeedHoldEnabled(bool enable);
    void setFeedOverrideEnabled(bool enable);
    void setAxisMaxPositionLimit(int axisIndex, double value);
    void setAxisMinPositionLimit(int axisIndex, double value);
    void setOptionalStopEnabled(bool enable);
    void setSpindleOverrideEnabled(bool enable);
    void setSpindle(SpindleMode mode);
    void setSpindle(SpindleMode mode, double velocity);
    void setSpindleOverride(double scale);
    void setTeleopEnabled(bool enable);
    void setTeleopVector(double a, double b, double c, double u, double v, double w);
    void setToolOffset(int index, double zOffset, double xOffset, double diameter, double frontangle, double backangle, int orientation);
    void setTrajectoryMode(TrajectoryMode mode);
    void unhomeAxis(int index);
    void shutdown();

private:
    bool m_connected;

private slots:
    void setConnected();
    void clearConnected();

signals:
    void connectedChanged(bool arg);

}; // class ApplicationCommand
} // namespace qtquickvcp

#endif // APPLICATIONCOMMAND_H
