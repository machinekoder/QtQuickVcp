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
#include "previewclient.h"
#include "glitem.h"
#include "glview.h"
#include "glcubeitem.h"
#include "glcylinderitem.h"
#include "glsphereitem.h"
#include "qglcamera.h"
#include "glpathitem.h"
#include "gllight.h"
#include "gllathetoolitem.h"
#include "glcanvas.h"
#include "gcodeprogrammodel.h"
#include "gcodeprogramloader.h"

static void initResources()
{
    Q_INIT_RESOURCE(pathview);
}

static const struct {
    const char *type;
    int major, minor;
} qmldir [] = {
    { "BoundingBox3D", 1, 0 },
    { "Coordinate3D", 1, 0 },
    { "ClearBackplotAction", 1, 0 },
    { "GCodeSync", 1, 0 },
    { "Grid3D", 1, 0 },
    { "PathView3D", 1, 0 },
    { "PathViewAction", 1, 0 },
    { "PathViewCore", 1, 0 },
    { "PathViewObject", 1, 0 },
    { "ProgramExtents3D", 1, 0 },
    { "ProgramOffsets3D", 1, 0 },
    { "SourceView", 1, 0 },
    { "ViewModeAction", 1, 0 },
    { "ZoomInAction", 1, 0 },
    { "ZoomOutAction", 1, 0 },
    { "ZoomOriginalAction", 1, 0 }
};

void MachinekitPathViewPlugin::registerTypes(const char *uri)
{
    initResources();

    // @uri Machinekit.PathView
    Q_ASSERT(uri == QLatin1String("Machinekit.PathView"));
    qmlRegisterType<QGLCamera>(uri, 1, 0, "Camera3D");
    qmlRegisterType<qtquickvcp::GLLight>(uri, 1, 0, "Light3D");
    qmlRegisterType<qtquickvcp::GLView>(uri, 1, 0, "GLView3D");
    qmlRegisterType<qtquickvcp::GLCubeItem>(uri, 1, 0, "Cube3D");
    qmlRegisterType<qtquickvcp::GLCylinderItem>(uri, 1, 0, "Cylinder3D");
    qmlRegisterType<qtquickvcp::GLSphereItem>(uri, 1, 0, "Sphere3D");
    qmlRegisterType<qtquickvcp::GLPathItem>(uri, 1, 0, "Path3D");
    qmlRegisterType<qtquickvcp::GLCanvas>(uri, 1, 0, "Canvas3D");
    qmlRegisterType<qtquickvcp::PreviewClient>(uri, 1, 0, "PreviewClient");
    qmlRegisterType<qtquickvcp::GCodeProgramModel>(uri, 1, 0, "GCodeProgramModel");
    qmlRegisterType<qtquickvcp::GCodeProgramLoader>(uri, 1, 0, "GCodeProgramLoader");
    qmlRegisterType<qtquickvcp::GLLatheToolItem>(uri, 1, 0, "LatheTool3D");

    const QString filesLocation = fileLocation();
    for (int i = 0; i < int(sizeof(qmldir)/sizeof(qmldir[0])); i++) {
        qmlRegisterType(QUrl(filesLocation + "/" + qmldir[i].type + ".qml"), uri, qmldir[i].major, qmldir[i].minor, qmldir[i].type);
    }
}

void MachinekitPathViewPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
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
        if (m_translator.load(QLocale(), QLatin1String("machinekitpathview"),
                              QLatin1String("_"), path))
        {
            QCoreApplication::installTranslator(&m_translator);
            break;
        }
    }
}

QString MachinekitPathViewPlugin::fileLocation() const
{
    if (isLoadedFromResource())
        return "qrc:/Machinekit/PathView";
    return baseUrl().toString();
}

bool MachinekitPathViewPlugin::isLoadedFromResource() const
{
    // If one file is missing, it will load all the files from the resource
    QFile file(baseUrl().toLocalFile() + "/" + qmldir[0].type + ".qml");
    return !file.exists();
}
