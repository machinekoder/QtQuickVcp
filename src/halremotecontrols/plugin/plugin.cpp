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
#include "plugin.h"

static void initResources()
{
    Q_INIT_RESOURCE(halremotecontrols);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "HalApplicationWindow", 1, 0 },
    { "HalButton", 1, 0 },
    { "HalCheckBox", 1, 0 },
    { "HalColorPicker", 1, 0 },
    { "HalComboBox", 1, 0 },
    { "HalContainer", 1, 0 },
    { "HalDial", 1, 0 },
    { "HalGauge", 1, 0 },
    { "HalKnob", 1, 0 },
    { "HalLabel", 1, 0 },
    { "HalLed", 1, 0 },
    { "HalLogChart", 1, 0 },
    { "HalProgressBar", 1, 0 },
    { "HalRadioButton", 1, 0 },
    { "HalRoundGauge", 1, 0 },
    { "HalSlider", 1, 0 },
    { "HalSpinBox", 1, 0 },
    { "HalSwitch", 1, 0 },
    { "HalVirtualJoystick", 1, 0 }
};

void MachinekitHalRemoteControlsPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.HalRemote.Controls
    Q_ASSERT(uri == QLatin1String("Machinekit.HalRemote.Controls"));
    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
        }
}

void MachinekitHalRemoteControlsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    if (isLoadedFromResource())
        engine->addImportPath(QStringLiteral("qrc:/"));

    // translate the plugin
    QVector<QString> paths;
    paths.push_back(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    paths.push_back(QCoreApplication::applicationDirPath());
    paths.push_back(QDir(QCoreApplication::applicationDirPath()).filePath("translations"));
    paths.push_back(QDir::currentPath());
    paths.push_back(QDir(QDir::currentPath()).filePath("translations"));

    for (const auto &path: paths) {
        if (m_translator.load(QLocale(), QLatin1String("machinekithalremotecontrols"),
                              QLatin1String("_"), path))
        {
            QCoreApplication::installTranslator(&m_translator);
            break;
        }
    }
}

QString MachinekitHalRemoteControlsPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/HalRemote/Controls";
    return baseUrl().toString();
}

bool MachinekitHalRemoteControlsPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/" + qmldir[0].type + ".qml");
    return !file.exists();
}
