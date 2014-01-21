/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: http://www.qt-project.org/
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**************************************************************************/

#include "positionernodeinstance.h"
#include <private/qquickpositioners_p.h>

namespace QmlDesigner {
namespace Internal {

PositionerNodeInstance::PositionerNodeInstance(QQuickBasePositioner *item)
    : QuickItemNodeInstance(item)
{
}

bool PositionerNodeInstance::isPositioner() const
{
    return true;
}

bool PositionerNodeInstance::isResizable() const
{
    return true;
}

void PositionerNodeInstance::setPropertyVariant(const QString &name, const QVariant &value)
{
    if (name == "move" || name == "add")
        return;

    QuickItemNodeInstance::setPropertyVariant(name, value);
}

void PositionerNodeInstance::setPropertyBinding(const QString &name, const QString &expression)
{
    if (name == "move" || name == "add")
        return;

    QuickItemNodeInstance::setPropertyBinding(name, expression);
}

PositionerNodeInstance::Pointer PositionerNodeInstance::create(QObject *object)
{ 
    QQuickBasePositioner *positioner = qobject_cast<QQuickBasePositioner*>(object);

    Q_ASSERT(positioner);

    Pointer instance(new PositionerNodeInstance(positioner));

    instance->setHasContent(anyItemHasContent(positioner));
    positioner->setFlag(QQuickItem::ItemHasContents, true);

    static_cast<QQmlParserStatus*>(positioner)->classBegin();

    instance->populateResetHashes();

    return instance;
}

QQuickBasePositioner *PositionerNodeInstance::positioner() const
{
    Q_ASSERT(qobject_cast<QQuickBasePositioner*>(object()));
    return static_cast<QQuickBasePositioner*>(object());
}

void PositionerNodeInstance::refreshPositioner()
{
    bool success = QMetaObject::invokeMethod(positioner(), "prePositioning");
    Q_ASSERT(success);
}

}
} // namespace QmlDesigner
