#include "halremotecomponent.h"
#include "debughelper.h"


namespace qtquickvcp {
/*!
    \qmltype HalRemoteComponent
    \instantiates qtquickvcp::HalRemoteComponent
    \inqmlmodule Machinekit.HalRemote
    \brief A HAL remote component.
    \ingroup halremote

    This component provides the counterpart of a HAL
    remote component in the HAL real-time environment.
    The HalRemoteComponent connects to a remote HAL instance
    using the halrcmd and halrcomp services provided by
    a Haltalk instance running on the remote host.

    A HalRemoteComponent needs the \l{halrcmdUri},
    \l{halrcompUri} and \l containerItem set in order
    to work.

    The HalRemoteComponent scans the \l containerItem
    and its children for \l{HalPin}s when \l ready is set
    to \c true.

    The following example creates a HAL remote component
    \c myComponent with one pin \c myPin. The resulting
    name for the pin inside HAL is \c myComponent.myPin.

    \qml
    Item {
        Item {
            id: container

            HalPin {
                name: "myPin"
                type: HalPin.Float
                direction: HalPin.Out
            }
        }

        HalRemoteComponent {
            id: halRemoteComponent

            name: "myComponent"
            halrcmdUri: "tcp://192.168.1.2:5001"
            halrcompUri: "tcp://192.168.1.2:5002"
            containerItem: container
            ready: true
        }
    }
    \endqml
*/

/*! \qmlproperty string HalRemoteComponent::halrcmdUri

    This property holds the halrcmd service uri.
*/

/*! \qmlproperty string HalRemoteComponent::halrcompUri

    This property holds the halrcomp service uri.
*/

/*! \qmlproperty string HalRemoteComponent::name

    This property holds the name of the remote component.
*/

/*! \qmlproperty int HalRemoteComponent::halrcmdHeartbeatInterval

    This property holds the period time of the heartbeat timer in ms.
    Set this property to \c{0} to disable the hearbeat.

    The default value is \c{3000}.
*/

/*! \qmlproperty int HalRemoteComponent::halrcompHeartbeatInterval

    This property holds the period time of the heartbeat timer in ms.
    This value is set automatically on the first full update.

    The default value is \c{0}.
*/

/*! \qmlproperty bool HalRemoteComponent::ready

    This property holds whether the HalRemoteComponent is ready or not.
    If the property is set to \c true the component will try to connect. If the
    property is set to \c false all connections will be closed.

    The default value is \c{false}.
*/

/*! \qmlproperty bool HalRemoteComponent::connected

    This property holds wheter the HAL remote component is connected or not. This is the
    same as \l{connectionState} == \c{HalRemoteComponent.Connected}.
 */

/*! \qmlproperty enumeration HalRemoteComponent::connectionState

    This property holds the connection state of the HAL remote component.

    \list
    \li HalRemoteComponent.Disconnected - The component is not connected.
    \li HalRemoteComponent.Connecting - The component is trying to connect to the remote component.
    \li HalRemoteComponent.Connected - The component is connected and pin changes are accepted.
    \li HalRemoteComponent.Error - An error has happened. See \l error and \l errorString for details about the error.
    \endlist
*/

/*! \qmlproperty enumeration HalRemoteComponent::error

    This property holds the currently active error. See \l errorString
    for a description of the active error.

    \list
    \li HalRemoteComponent.NoError - No error happened.
    \li HalRemoteComponent.ComponentError - An error happened.
    \endlist

    \sa errorString
*/

/*! \qmlproperty string HalRemoteComponent::errorString

    This property holds a text description of the last error that occured.
    If \l error holds a error value check this property for the description.

    \sa error
*/

/*! \qmlproperty Item HalRemoteComponent::containerItem

    This property holds the item that should be scanned for
    \l{HalPin}s.

    The default value is \c{nullptr}.
*/

/*! \qmlproperty Item HalRemoteComponent::create

    Specifies wether the component should be created on bind if it
    does not exist on the remote host.

    The default value is \c{true}.
*/

/*! \qmlproperty Item HalRemoteComponent::bind

    Specifies wether the component should be bound when connecting. If not the
    remote representation of the pins is refrelcted by \c{pins}.

    The default value is \c{true}.
*/

/*! \qmlproperty list<HalPin> HalRemoteComponent::pins

    This property holds a list of HAL pins when bound or connected.
*/

/** Remote HAL Component implementation for use with C++ and QML */
HalRemoteComponent::HalRemoteComponent(QObject *parent) :
    QObject(parent),
    m_name("default"),
    m_connected(false),
    m_error(NoError),
    m_containerItem(this),
    m_create(true),
    m_bind(true)
{
}

/** Updates a remote pin witht the value of a local pin */
void HalRemoteComponent::pinChange(QVariant value)
{
}

/** Recurses through a list of objects */
const QObjectList HalRemoteComponent::recurseObjects(const QObjectList &list)
{
    QObjectList halObjects;

    for (QObject *object: list)
    {
        HalPin *halPin = qobject_cast<HalPin *>(object);
        if (halPin != nullptr)
        {
            halObjects.append(object);
        }

        if (object->children().size() > 0)
        {
            halObjects.append(recurseObjects(object->children()));
        }
    }

    return halObjects;
}


/** splits HAL pin name from full HAL name **/
QString HalRemoteComponent::splitPinFromHalName(const QString &name)
{
    QString newName;
    int dotIndex = name.indexOf(".");
    if (dotIndex != -1)    // strip comp prefix
    {
        newName = name.mid(dotIndex + 1);
    }
    else
    {
        newName = name;
    }
    return newName;
}

/** Scans all children of the container item for pins and adds them to a map */
void HalRemoteComponent::addPins()
{

}

/** Removes all previously added pins */
void HalRemoteComponent::removePins()
{
}

/** Sets synced of all pins to false */
void HalRemoteComponent::unsyncPins()
{

}

void HalRemoteComponent::bindComponent()
{
}

void HalRemoteComponent::setConnected()
{
    m_connected = true;
    emit connectedChanged(m_connected);
}

void HalRemoteComponent::setError()
{
    if (m_error != ComponentError) {
        m_error = ComponentError;
        emit errorChanged(m_error);
    }

    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setDisconnected()
{
    if (m_error != NoError) {
        m_error = NoError;
        emit errorChanged(m_error);
    }

    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setConnecting()
{
    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

void HalRemoteComponent::setTimeout()
{
    if (m_connected)
    {
        m_connected = false;
        emit connectedChanged(m_connected);
    }
}

} // namespace qtquickvcp
