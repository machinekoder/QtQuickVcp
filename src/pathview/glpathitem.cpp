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

#include "glpathitem.h"
#include <QtCore/qmath.h>
#include <cmath>
#include "debughelper.h"

const float PI_F = 3.14159265358979f;
const float PI_2_F = 1.57079632679489f;

namespace qtquickvcp {

GLPathItem::GLPathItem(QQuickItem *parent)
    : GLItem(parent)
    , m_model(nullptr)
    , m_arcFeedColor(QColor(Qt::white))
    , m_straightFeedColor(QColor(Qt::white))
    , m_traverseColor(QColor(Qt::cyan))
    , m_backplotArcFeedColor(QColor(Qt::yellow))
    , m_backplotStraightFeedColor(QColor(Qt::yellow))
    , m_backplotTraverseColor(QColor(Qt::yellow))
    , m_selectedColor(QColor(Qt::magenta))
    , m_activeColor(QColor(Qt::red))
    , m_lineWidth(1.0)
    , m_traverseLineStippleLength(1.0)
    , m_arcDivision(16)
    , m_activePlane(XYPlane)
    , m_needsFullUpdate(true)
    , m_minimumExtents(QVector3D(0, 0, 0))
    , m_maximumExtents(QVector3D(0, 0, 0))
    , m_extentsUpdated(false)
{
    connect(this, &GLPathItem::visibleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::positionChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::scaleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationAngleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::rotationAxisChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::visibleChanged,
            this, &GLPathItem::triggerFullUpdate);
    connect(this, &GLPathItem::lineWidthChanged,
            this, &GLPathItem::triggerFullUpdate);
}

GLPathItem::~GLPathItem()
{
    qDeleteAll(m_previewPathItems);
}

void GLPathItem::paint(GLView *glView)
{
    if (m_needsFullUpdate)
    {
        glView->prepare(this);
        glView->reset();
        glView->lineWidth(m_lineWidth);
        glView->beginUnion();

        for (int i = 0; i < m_previewPathItems.size(); ++i)
        {
            void* drawablePointer = nullptr;
            PathItem *pathItem = m_previewPathItems.at(i);
            if (pathItem->pathType == Line)
            {
                LinePathItem *linePathItem = static_cast<LinePathItem*>(pathItem);
                if (linePathItem->movementType == FeedMove)
                {
                    glView->color(m_straightFeedColor);
                }
                else
                {
                    glView->color(m_traverseColor);
                    glView->lineStipple(true, m_traverseLineStippleLength);
                }
                glView->translate(linePathItem->position);
                drawablePointer = glView->line(linePathItem->lineVector);
            }
            else if (pathItem->pathType == Arc)
            {
                ArcPathItem *arcPathItem = static_cast<ArcPathItem*>(pathItem);
                glView->color(m_arcFeedColor);
                glView->translate(arcPathItem->position);
                if (arcPathItem->rotationPlane == XZPlane) {
                    glView->rotate(-90, 0, 1, 0);
                    glView->rotate(-90, 1, 0, 0);
                }
                else if  (arcPathItem->rotationPlane == YZPlane) {
                    glView->rotate(90, 1, 0, 0);
                    glView->rotate(90, 0, 1, 0);
                }
                drawablePointer = glView->arc(arcPathItem->center.x(),
                                              arcPathItem->center.y(),
                                              arcPathItem->radius,
                                              arcPathItem->startAngle,
                                              arcPathItem->endAngle,
                                              arcPathItem->anticlockwise,
                                              arcPathItem->helixOffset,
                                              m_arcDivision);
            }

            if (drawablePointer != nullptr)
            {
                pathItem->drawablePointer = drawablePointer;
                m_drawablePathMap.insert(drawablePointer, pathItem);
            }
        }

        glView->endUnion();

        m_needsFullUpdate = false;
    }
    else
    {
        for (PathItem *pathItem: qAsConst(m_modifiedPathItems))
        {
            if (pathItem != nullptr)
            {
                QColor color;
                if (m_model->data(pathItem->modelIndex, GCodeProgramModel::SelectedRole).toBool()) {
                    color = m_selectedColor;
                }
                else if (m_model->data(pathItem->modelIndex, GCodeProgramModel::ActiveRole).toBool())
                {
                    color = m_activeColor;
                }
                else if (m_model->data(pathItem->modelIndex, GCodeProgramModel::ExecutedRole).toBool())
                {
                    if (pathItem->movementType == FeedMove) {
                        if (pathItem->pathType == Arc) {
                            color = m_backplotArcFeedColor;
                        }
                        else {
                            color = m_backplotStraightFeedColor;
                        }
                    }
                    else {
                        color = m_backplotTraverseColor;
                    }
                }
                else
                {
                    if (pathItem->movementType == FeedMove) {
                        if (pathItem->pathType == Arc) {
                            color = m_arcFeedColor;
                        }
                        else {
                            color = m_straightFeedColor;
                        }
                    }
                    else {
                        color = m_traverseColor;
                    }
                }
                glView->updateColor(pathItem->drawablePointer, color);
            }
        }
        m_modifiedPathItems.clear();
    }
}

GCodeProgramModel *GLPathItem::model() const
{
    return m_model;
}

QColor GLPathItem::arcFeedColor() const
{
    return m_arcFeedColor;
}

QColor GLPathItem::traverseColor() const
{
    return m_traverseColor;
}

QColor GLPathItem::selectedColor() const
{
    return m_selectedColor;
}

QVector3D GLPathItem::minimumExtents() const
{
    return m_minimumExtents;
}

QVector3D GLPathItem::maximumExtents() const
{
    return m_maximumExtents;
}

float GLPathItem::lineWidth() const
{
    return m_lineWidth;
}

float GLPathItem::traverseLineStippleLength() const
{
    return m_traverseLineStippleLength;
}

int GLPathItem::arcDivision() const
{
    return m_arcDivision;
}

QColor GLPathItem::straightFeedColor() const
{
    return m_straightFeedColor;
}

QColor GLPathItem::activeColor() const
{
    return m_activeColor;
}

QColor GLPathItem::backplotArcFeedColor() const
{
    return m_backplotArcFeedColor;
}

QColor GLPathItem::backplotStraightFeedColor() const
{
    return m_backplotStraightFeedColor;
}

QColor GLPathItem::backplotTraverseColor() const
{
    return m_backplotTraverseColor;
}

void GLPathItem::selectDrawable(void *pointer)
{
    if (m_model == nullptr) {
        return;
    }

    PathItem *mappedPathItem = m_drawablePathMap.value(pointer, nullptr);
    if (mappedPathItem == nullptr) {
        return;
    }

    const QModelIndex &mappedModelIndex = mappedPathItem->modelIndex;
    m_model->clearSelectionAndSelectLine(mappedModelIndex);
    m_modifiedPathItems.append(mappedPathItem);
    emit needsUpdate();
}

void GLPathItem::setModel(GCodeProgramModel *arg)
{
    if (m_model != arg) {
        m_model = arg;
        emit modelChanged(arg);

        if (m_model != nullptr)
        {
            connect(m_model, &QAbstractItemModel::modelReset,
                    this, &GLPathItem::drawPath);
            connect(m_model, &QAbstractItemModel::dataChanged,
                    this, &GLPathItem::modelDataChanged);

            if (m_model->rowCount() > 0)
            {
                drawPath();     // draw model when set
            }
        }
    }
}

void GLPathItem::setArcFeedColor(QColor arg)
{
    if (m_arcFeedColor != arg) {
        m_arcFeedColor = arg;
        emit arcFeedColorChanged(arg);
    }
}

void GLPathItem::setTraverseColor(QColor arg)
{
    if (m_traverseColor != arg) {
        m_traverseColor = arg;
        emit traverseColorChanged(arg);
    }
}

void GLPathItem::setSelectedColor(QColor arg)
{
    if (m_selectedColor != arg) {
        m_selectedColor = arg;
        emit selectedColorChanged(arg);
    }
}

void GLPathItem::setStraightFeedColor(QColor arg)
{
    if (m_straightFeedColor != arg) {
        m_straightFeedColor = arg;
        emit straightFeedColorChanged(arg);
    }
}

void GLPathItem::setActiveColor(QColor arg)
{
    if (m_activeColor != arg) {
        m_activeColor = arg;
        emit activeColorChanged(arg);
    }
}

void GLPathItem::setLineWidth(float arg)
{
    if (m_lineWidth != arg) {
        m_lineWidth = arg;
        emit lineWidthChanged(arg);
    }
}

void GLPathItem::setTraverseLineStippleLength(float arg)
{
    if (m_traverseLineStippleLength != arg) {
        m_traverseLineStippleLength = arg;
        emit traverseLineStippleLengthChanged(arg);
    }
}

void GLPathItem::setArcDivision(int arcDivision)
{
    if (m_arcDivision == arcDivision) {
        return;
    }

    m_arcDivision = arcDivision;
    emit arcDivisionChanged(arcDivision);
}

void GLPathItem::setBackplotArcFeedColor(QColor arg)
{
    if (m_backplotArcFeedColor != arg) {
        m_backplotArcFeedColor = arg;
        emit backplotArcFeedColorChanged(arg);
    }
}

void GLPathItem::setBackplotStraightFeedColor(QColor arg)
{
    if (m_backplotStraightFeedColor != arg) {
        m_backplotStraightFeedColor = arg;
        emit backplotStraightFeedColorChanged(arg);
    }
}

void GLPathItem::setBackplotTraverseColor(QColor arg)
{
    if (m_backplotTraverseColor != arg) {
        m_backplotTraverseColor = arg;
        emit backplotTraverseColorChanged(arg);
    }
}

void GLPathItem::resetActiveOffsets()
{
    Position clearOffset;
    clearOffset.x = 0.0;
    clearOffset.y = 0.0;
    clearOffset.z = 0.0;
    clearOffset.a = 0.0;
    clearOffset.b = 0.0;
    clearOffset.c = 0.0;
    clearOffset.u = 0.0;
    clearOffset.v = 0.0;
    clearOffset.w = 0.0;

    m_activeOffsets.g92Offset = clearOffset;
    m_activeOffsets.toolOffset = clearOffset;
    m_activeOffsets.g5xOffsets.clear();
    for (int i = 0; i < 9; ++i)
    {
        m_activeOffsets.g5xOffsets.append(clearOffset);
    }
    m_activeOffsets.g5xOffsetIndex = 1;
}

void GLPathItem::resetCurrentPosition()
{
    m_currentPosition.x = 0.0;
    m_currentPosition.y = 0.0;
    m_currentPosition.z = 0.0;
    m_currentPosition.a = 0.0;
    m_currentPosition.b = 0.0;
    m_currentPosition.c = 0.0;
    m_currentPosition.u = 0.0;
    m_currentPosition.v = 0.0;
    m_currentPosition.w = 0.0;
}

void GLPathItem::resetRelativePosition()
{
}

void GLPathItem::resetActivePlane()
{
    m_activePlane = XYPlane;
}

void GLPathItem::resetExtents()
{
    m_maximumExtents = QVector3D();
    m_minimumExtents = QVector3D();
    m_extentsUpdated = false;
}

void GLPathItem::updateExtents(const QVector3D &vector)
{
    if ((vector.x() < m_minimumExtents.x()) || !m_extentsUpdated) {
        m_minimumExtents.setX(vector.x());
    }
    if ((vector.y() < m_minimumExtents.y()) || !m_extentsUpdated) {
        m_minimumExtents.setY(vector.y());
    }
    if ((vector.z() < m_minimumExtents.z()) || !m_extentsUpdated) {
        m_minimumExtents.setZ(vector.z());
    }
    if ((vector.x() > m_maximumExtents.x()) || !m_extentsUpdated) {
        m_maximumExtents.setX(vector.x());
    }
    if ((vector.y() > m_maximumExtents.y()) || !m_extentsUpdated) {
        m_maximumExtents.setY(vector.y());
    }
    if ((vector.z() > m_maximumExtents.z()) || !m_extentsUpdated) {
        m_maximumExtents.setZ(vector.z());
    }
    m_extentsUpdated = true;
}

void GLPathItem::releaseExtents()
{
    emit minimumExtentsChanged(m_minimumExtents);
    emit maximumExtentsChanged(m_maximumExtents);
}

QVector3D GLPathItem::positionToVector3D(const GLPathItem::Position &position) const
{
    return QVector3D(position.x, position.y, position.z);
}

void GLPathItem::drawPath()
{
    if (m_model == nullptr)
    {
        return;
    }

    qDeleteAll(m_previewPathItems); // clear the list of preview path items
    m_previewPathItems.clear();
    resetActiveOffsets(); // clear the offsets
    resetActivePlane();
    resetCurrentPosition();  // reset current position
    resetRelativePosition(); // reset relative position
    resetExtents();

    m_modelPathMap.clear();
    m_drawablePathMap.clear();

    QLinkedList<GCodeProgramModel::PreviewItem> previewItems = m_model->previewItems();
    QLinkedListIterator<GCodeProgramModel::PreviewItem> i(previewItems);
    while (i.hasNext())
    {
        GCodeProgramModel::PreviewItem item = i.next();
        m_currentModelIndex = item.modelIndex;
    }

    m_needsFullUpdate = true;
    emit needsUpdate();

    releaseExtents();
}

void GLPathItem::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    Q_UNUSED(bottomRight) // we only change one item at a time
    if (roles.contains(GCodeProgramModel::SelectedRole)
        || roles.contains(GCodeProgramModel::ActiveRole)
        || roles.contains(GCodeProgramModel::ExecutedRole))
    {
        QList<PathItem*> pathItemList;

        pathItemList = m_modelPathMap.values(topLeft);
        if (!pathItemList.isEmpty())
        {
            m_modifiedPathItems.append(pathItemList);
            emit needsUpdate();
        }
    }
}

void GLPathItem::triggerFullUpdate()
{
    m_needsFullUpdate = true;
    emit needsUpdate();
}
} // namespace qtquickvcp
