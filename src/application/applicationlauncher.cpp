#include "applicationlauncher.h"
#include <machinetalkservice.h>
#include "debughelper.h"

namespace qtquickvcp {

ApplicationLauncher::ApplicationLauncher(QObject *parent) :
    QObject(parent),
    m_launcherObject(nullptr),
    m_synced(false),
    m_temporaryDir(nullptr)
{
    initializeObject();
    createTemporaryDir();
}

ApplicationLauncher::~ApplicationLauncher()
{
}

QVariant ApplicationLauncher::launchers() const
{
    if (m_launcherObject != nullptr) {
        return m_launcherObject->property("launcher");
    }
    else {
        return QVariant();
    }
}

bool ApplicationLauncher::isSynced() const
{
    return m_synced;
}

void ApplicationLauncher::start(int index)
{
    if (!m_synced) {
        return;
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, debugName(), "starting launcher" << index)
#endif
}

void ApplicationLauncher::kill(int index)
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::terminate(int index)
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::writeToStdin(int index, const QString &data)
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::call(const QString &command)
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::shutdown()
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::setImportance(int index, int importance)
{
    if (!m_synced) {
        return;
    }
}

void ApplicationLauncher::syncStatus()
{
    m_synced = true;
    emit syncedChanged(m_synced);
}

void ApplicationLauncher::unsyncStatus()
{
    m_synced = false;
    emit syncedChanged(m_synced);
    initializeObject();
    emit launchersChanged();
}

void ApplicationLauncher::initializeObject()
{
    if (m_launcherObject != nullptr) {
        m_launcherObject->deleteLater();
    }
}

void ApplicationLauncher::createTemporaryDir()
{
}

} // namespace qtquickvcp
