/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QMLSTATENODEINSTANCE_H
#define QMLSTATENODEINSTANCE_H

#include "objectnodeinstance.h"

QT_BEGIN_NAMESPACE
class QQuickState;
class QQuickStateGroup;
QT_END_NAMESPACE

namespace QmlDesigner {

namespace Internal {

class QmlStateNodeInstance : public ObjectNodeInstance
{
public:
    typedef QSharedPointer<QmlStateNodeInstance> Pointer;
    typedef QWeakPointer<QmlStateNodeInstance> WeakPointer;

    static Pointer create(QObject *objectToBeWrapped);

    void setPropertyVariant(const QString &name, const QVariant &value);
    void setPropertyBinding(const QString &name, const QString &expression);

    void activateState();
    void deactivateState();

    bool updateStateVariant(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QVariant &value);
    bool updateStateBinding(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QString &expression);
    bool resetStateProperty(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QVariant &resetValue);


protected:

    QmlStateNodeInstance(QQuickState *object);

    bool isStateActive() const;

    QQuickState *stateObject() const;
    QQuickStateGroup *stateGroup() const;
};

} // namespace Internal
} // namespace QmlDesigner

#endif // QMLSTATENODEINSTANCE_H
