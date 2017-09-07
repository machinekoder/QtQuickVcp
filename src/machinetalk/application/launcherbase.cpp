/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "launcherbase.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk { namespace application {

/** Generic Launcher Base implementation */
LauncherBase::LauncherBase(QObject *parent)
    : QObject(parent)
    , QQmlParserStatus()
    , m_componentCompleted(false)
    , m_ready(false)
    , m_debugName("Launcher Base")
    , m_launchercmdChannel(nullptr)
    , m_launcherChannel(nullptr)
    , m_state(State::Down)
    , m_previousState(State::Down)
    , m_errorString("")
{
    // initialize launchercmd channel
    m_launchercmdChannel = new common::RpcClient(this);
    m_launchercmdChannel->setDebugName(m_debugName + " - launchercmd");
    connect(m_launchercmdChannel, &common::RpcClient::socketUriChanged,
            this, &LauncherBase::launchercmdUriChanged);
    connect(m_launchercmdChannel, &common::RpcClient::stateChanged,
            this, &LauncherBase::launchercmdChannelStateChanged);
    connect(m_launchercmdChannel, &common::RpcClient::socketMessageReceived,
            this, &LauncherBase::processLaunchercmdChannelMessage);
    // initialize launcher channel
    m_launcherChannel = new application::LauncherSubscribe(this);
    m_launcherChannel->setDebugName(m_debugName + " - launcher");
    connect(m_launcherChannel, &application::LauncherSubscribe::socketUriChanged,
            this, &LauncherBase::launcherUriChanged);
    connect(m_launcherChannel, &application::LauncherSubscribe::stateChanged,
            this, &LauncherBase::launcherChannelStateChanged);
    connect(m_launcherChannel, &application::LauncherSubscribe::socketMessageReceived,
            this, &LauncherBase::processLauncherChannelMessage);

    connect(m_launchercmdChannel, &common::RpcClient::heartbeatIntervalChanged,
            this, &LauncherBase::launchercmdHeartbeatIntervalChanged);

    connect(m_launcherChannel, &application::LauncherSubscribe::heartbeatIntervalChanged,
            this, &LauncherBase::launcherHeartbeatIntervalChanged);
    // state machine
    connect(this, &LauncherBase::fsmSyncedEntered,
            this, &LauncherBase::fsmSyncedEntry);
    connect(this, &LauncherBase::fsmSyncedExited,
            this, &LauncherBase::fsmSyncedExit);
    connect(this, &LauncherBase::fsmDownConnect,
            this, &LauncherBase::fsmDownConnectEvent);
    connect(this, &LauncherBase::fsmTryingLaunchercmdUp,
            this, &LauncherBase::fsmTryingLaunchercmdUpEvent);
    connect(this, &LauncherBase::fsmTryingDisconnect,
            this, &LauncherBase::fsmTryingDisconnectEvent);
    connect(this, &LauncherBase::fsmSyncingLaunchercmdTrying,
            this, &LauncherBase::fsmSyncingLaunchercmdTryingEvent);
    connect(this, &LauncherBase::fsmSyncingLauncherUp,
            this, &LauncherBase::fsmSyncingLauncherUpEvent);
    connect(this, &LauncherBase::fsmSyncingDisconnect,
            this, &LauncherBase::fsmSyncingDisconnectEvent);
    connect(this, &LauncherBase::fsmSyncedLauncherTrying,
            this, &LauncherBase::fsmSyncedLauncherTryingEvent);
    connect(this, &LauncherBase::fsmSyncedLaunchercmdTrying,
            this, &LauncherBase::fsmSyncedLaunchercmdTryingEvent);
    connect(this, &LauncherBase::fsmSyncedDisconnect,
            this, &LauncherBase::fsmSyncedDisconnectEvent);
}

LauncherBase::~LauncherBase()
{
}

/** Add a topic that should be subscribed **/
void LauncherBase::addLauncherTopic(const QString &name)
{
    m_launcherChannel->addSocketTopic(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void LauncherBase::removeLauncherTopic(const QString &name)
{
    m_launcherChannel->removeSocketTopic(name);
}

/** Clears the the topics that should be subscribed **/
void LauncherBase::clearLauncherTopics()
{
    m_launcherChannel->clearSocketTopics();
}

void LauncherBase::startLaunchercmdChannel()
{
    m_launchercmdChannel->setReady(true);
}

void LauncherBase::stopLaunchercmdChannel()
{
    m_launchercmdChannel->setReady(false);
}

void LauncherBase::startLauncherChannel()
{
    m_launcherChannel->setReady(true);
}

void LauncherBase::stopLauncherChannel()
{
    m_launcherChannel->setReady(false);
}

/** Processes all message received on launchercmd */
void LauncherBase::processLaunchercmdChannelMessage(const Container &rx)
{

    // react to error message
    if (rx.type() == MT_ERROR)
    {

        // update error string with note
        m_errorString = "";
        for (int i = 0; i < rx.note_size(); ++i)
        {
            m_errorString.append(QString::fromStdString(rx.note(i)) + "\n");
        }
        emit errorStringChanged(m_errorString);
    }

    emit launchercmdMessageReceived(rx);
}

/** Processes all message received on launcher */
void LauncherBase::processLauncherChannelMessage(const QByteArray &topic, const Container &rx)
{

    // react to launcher full update message
    if (rx.type() == MT_LAUNCHER_FULL_UPDATE)
    {
        launcherFullUpdateReceived(topic, rx);
    }

    // react to launcher incremental update message
    else if (rx.type() == MT_LAUNCHER_INCREMENTAL_UPDATE)
    {
        launcherIncrementalUpdateReceived(topic, rx);
    }

    emit launcherMessageReceived(topic, rx);
}

void LauncherBase::sendLaunchercmdMessage(ContainerType type, Container &tx)
{
    m_launchercmdChannel->sendSocketMessage(type, tx);
}

void LauncherBase::sendLauncherStart(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_START, tx);
}

void LauncherBase::sendLauncherKill(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_KILL, tx);
}

void LauncherBase::sendLauncherTerminate(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_TERMINATE, tx);
}

void LauncherBase::sendLauncherWriteStdin(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_WRITE_STDIN, tx);
}

void LauncherBase::sendLauncherCall(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_CALL, tx);
}

void LauncherBase::sendLauncherShutdown(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_SHUTDOWN, tx);
}

void LauncherBase::sendLauncherSet(Container &tx)
{
    sendLaunchercmdMessage(MT_LAUNCHER_SET, tx);
}

void LauncherBase::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = State::Down;
    emit stateChanged(m_state);
}

void LauncherBase::fsmDownConnectEvent()
{
    if (m_state == State::Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event CONNECT");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        startLaunchercmdChannel();
     }
}

void LauncherBase::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = State::Trying;
    emit stateChanged(m_state);
}

void LauncherBase::fsmTryingLaunchercmdUpEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event LAUNCHERCMD UP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmSyncing();
        emit fsmSyncingEntered(QPrivateSignal());
        // execute actions
        startLauncherChannel();
     }
}

void LauncherBase::fsmTryingDisconnectEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopLaunchercmdChannel();
        stopLauncherChannel();
     }
}

void LauncherBase::fsmSyncing()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State SYNCING");
#endif
    m_state = State::Syncing;
    emit stateChanged(m_state);
}

void LauncherBase::fsmSyncingLaunchercmdTryingEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event LAUNCHERCMD TRYING");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopLauncherChannel();
     }
}

void LauncherBase::fsmSyncingLauncherUpEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event LAUNCHER UP");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmSynced();
        emit fsmSyncedEntered(QPrivateSignal());
        // execute actions
     }
}

void LauncherBase::fsmSyncingDisconnectEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopLaunchercmdChannel();
        stopLauncherChannel();
     }
}

void LauncherBase::fsmSynced()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State SYNCED");
#endif
    m_state = State::Synced;
    emit stateChanged(m_state);
}
void LauncherBase::fsmSyncedEntry()
{
    syncStatus();
}
void LauncherBase::fsmSyncedExit()
{
    unsyncStatus();
}

void LauncherBase::fsmSyncedLauncherTryingEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event LAUNCHER TRYING");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmSyncing();
        emit fsmSyncingEntered(QPrivateSignal());
        // execute actions
     }
}

void LauncherBase::fsmSyncedLaunchercmdTryingEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event LAUNCHERCMD TRYING");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopLauncherChannel();
     }
}

void LauncherBase::fsmSyncedDisconnectEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopLaunchercmdChannel();
        stopLauncherChannel();
     }
}

void LauncherBase::launchercmdChannelStateChanged(common::RpcClient::State state)
{

    if (state == common::RpcClient::State::Trying)
    {
        if (m_state == State::Syncing)
        {
            emit fsmSyncingLaunchercmdTrying(QPrivateSignal());
        }
        else if (m_state == State::Synced)
        {
            emit fsmSyncedLaunchercmdTrying(QPrivateSignal());
        }
    }

    else if (state == common::RpcClient::State::Up)
    {
        if (m_state == State::Trying)
        {
            emit fsmTryingLaunchercmdUp(QPrivateSignal());
        }
    }
}

void LauncherBase::launcherChannelStateChanged(application::LauncherSubscribe::State state)
{

    if (state == application::LauncherSubscribe::State::Trying)
    {
        if (m_state == State::Synced)
        {
            emit fsmSyncedLauncherTrying(QPrivateSignal());
        }
    }

    else if (state == application::LauncherSubscribe::State::Up)
    {
        if (m_state == State::Syncing)
        {
            emit fsmSyncingLauncherUp(QPrivateSignal());
        }
    }
}

/** start trigger function */
void LauncherBase::start()
{
    if (m_state == State::Down) {
        emit fsmDownConnect(QPrivateSignal());
    }
}

/** stop trigger function */
void LauncherBase::stop()
{
    if (m_state == State::Trying) {
        emit fsmTryingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Syncing) {
        emit fsmSyncingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Synced) {
        emit fsmSyncedDisconnect(QPrivateSignal());
    }
}

} } // namespace machinetalk::application
