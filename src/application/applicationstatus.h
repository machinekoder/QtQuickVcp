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
#ifndef APPLICATIONSTATUS_H
#define APPLICATIONSTATUS_H

#include <QObject>
#include <QJsonObject>

namespace qtquickvcp {

class ApplicationStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* config READ config NOTIFY configChanged)
    Q_PROPERTY(QObject* motion READ motion NOTIFY motionChanged)
    Q_PROPERTY(QObject* io READ io NOTIFY ioChanged)
    Q_PROPERTY(QObject* task READ task NOTIFY taskChanged)
    Q_PROPERTY(QObject* interp READ interp NOTIFY interpChanged)
    Q_PROPERTY(QObject* ui READ ui NOTIFY uiChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(bool synced READ isSynced NOTIFY syncedChanged)
    Q_PROPERTY(StatusChannels channels READ channels WRITE setChannels NOTIFY channelsChanged)
    Q_PROPERTY(StatusChannels optionalChannels READ optionalChannels WRITE setOptionalChannels NOTIFY optionalChannelsChanged)
    Q_FLAGS(StatusChannels)

public:
    explicit ApplicationStatus(QObject *parent = 0);

    enum OriginIndex {
        OriginG54,
        OriginG55,
        OriginG56,
        OriginG57,
        OriginG58,
        OriginG59,
        OriginG59_1,
        OriginG59_2,
        OriginG59_3
    };
    Q_ENUM(OriginIndex)

    enum TrajectoryMode {
        FreeMode,
        CoordinatedMode,
        TeleopMode
    };
    Q_ENUM(TrajectoryMode)

    enum MotionStatus {
        MotionUninitialized,
        MotionDone,
        MotionExec,
        MotionError,
        MotionReceived
    };
    Q_ENUM(MotionStatus)

    enum MotionType {
        NoneType,
        TraverseType,
        FeedType,
        ArcType,
        ToolchangeType,
        ProbingType,
        IndexRotaryType
    };
    Q_ENUM(MotionType)

    enum AxisType {
        LinearAxis,
        AngularAxis
    };
    Q_ENUM(AxisType)

    enum KinematicsType {
        IdentityKinematics,
        ForwardOnlyKinematics,
        InverseOnlyKinematics,
        BothKinematics
    };
    Q_ENUM(KinematicsType)

    enum CanonUnits {
        CanonUnitsInch,
        CanonUnitsMm,
        CanonUnitsCm
    };
    Q_ENUM(CanonUnits)

    enum TimeUnits {
        TimeUnitsMinute,
        TimeUnitsSecond
    };
    Q_ENUM(TimeUnits)

    enum TaskExecState {
        TaskError,
        TaskDone,
        TaskWaitingForMotion,
        TaskWaitingForMotionQueue,
        TaskWaitingForIo,
        TaskWaitingForMotionAndIo,
        TaskWaitingForDelay,
        TaskWaitingForSystemCmd,
        TaskWaitingForSpindleOriented
    };
    Q_ENUM(TaskExecState)

    enum TaskMode {
        TaskModeManual,
        TaskModeAuto,
        TaskModeMdi
    };
    Q_ENUM(TaskMode)

    enum TaskState {
        TaskStateEstop,
        TaskStateEstopReset,
        TaskStateOff,
        TaskStateOn
    };
    Q_ENUM(TaskState)

    enum InterpreterState {
        InterpreterIdle,
        InterpreterReading,
        InterpreterPaused,
        InterpreterWaiting
    };
    Q_ENUM(InterpreterState)

    enum InterpreterExitCode {
        InterpreterExitOk,
        InterpreterExitExit,
        InterpreterExitExecuteFinish,
        InterpreterExitEndfile,
        InterpreterExitFileNotOpen,
        InterpreterExitError
    };
    Q_ENUM(InterpreterExitCode)

    enum PositionOffset {
        RelativePositionOffset,
        MachinePositionOffset
    };
    Q_ENUM(PositionOffset)

    enum PositionFeedback {
        ActualPositionFeedback,
        CommandedPositionFeedback
    };
    Q_ENUM(PositionFeedback)

    enum StatusChannel {
        NoChannel = 0x0,
        MotionChannel = 0x1,
        ConfigChannel = 0x2,
        IoChannel     = 0x4,
        TaskChannel   = 0x8,
        InterpChannel = 0x10,
        UiChannel     = 0x20
    };
    Q_FLAG(StatusChannel)
    Q_DECLARE_FLAGS(StatusChannels, StatusChannel)

    void componentComplete();

    QObject* config() const
    {
        return m_config;
    }

    QObject* motion() const
    {
        return m_motion;
    }

    QObject* io() const
    {
        return m_io;
    }

    QObject* task() const
    {
        return m_task;
    }

    QObject* interp() const
    {
        return m_interp;
    }

    QObject* ui() const {
        return m_ui;
    }

    StatusChannels channels() const
    {
        return m_channels;
    }

    StatusChannels optionalChannels() const
    {
        return m_optionalChannels;
    }

    bool isRunning() const
    {
        return m_running;
    }

    bool isSynced() const
    {
        return m_synced;
    }

public slots:
    void setChannels(StatusChannels arg)
    {
        if (m_channels == arg)
            return;

        m_channels = arg;
        emit channelsChanged(arg);
    }

    void setOptionalChannels(StatusChannels optionalChannels)
    {
        if (m_optionalChannels == optionalChannels)
            return;

        m_optionalChannels = optionalChannels;
        emit optionalChannelsChanged(m_optionalChannels);
    }

private:
    QObject     *m_config;
    QObject     *m_motion;
    QObject     *m_io;
    QObject     *m_task;
    QObject     *m_interp;
    QObject     *m_ui;
    bool            m_running;
    bool            m_synced;
    StatusChannels  m_syncedChannels;
    StatusChannels  m_channels;
    StatusChannels  m_optionalChannels;

    void updateSync(StatusChannel channel);
    void initializeObject(StatusChannel channel);


private slots:
    void syncStatus();
    void unsyncStatus();
    void updateTopics();

    void updateRunning();

signals:
    void configChanged();
    void motionChanged();
    void ioChanged();
    void taskChanged();
    void interpChanged();
    void uiChanged();
    void channelsChanged(StatusChannels arg);
    void optionalChannelsChanged(StatusChannels optionalChannels);
    void runningChanged(bool arg);
    void syncedChanged(bool arg);
}; // class ApplicationStatus
} // namespace qtquickvcp

#endif // APPLICATIONSTATUS_H
