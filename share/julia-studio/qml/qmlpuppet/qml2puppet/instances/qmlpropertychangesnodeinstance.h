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

#ifndef QMLPROPERTYCHANGESNODEINSTANCE_H
#define QMLPROPERTYCHANGESNODEINSTANCE_H

#include "objectnodeinstance.h"
#include <private/qquickstateoperations_p.h>
#include <private/qquickpropertychanges_p.h>

#include <QPair>
#include <QWeakPointer>

QT_BEGIN_NAMESPACE
class QQuickProperty;
QT_END_NAMESPACE

namespace QmlDesigner {

namespace Internal {

class QmlPropertyChangesNodeInstance;

class QmlPropertyChangesNodeInstance : public ObjectNodeInstance
{
public:
    typedef QSharedPointer<QmlPropertyChangesNodeInstance> Pointer;
    typedef QWeakPointer<QmlPropertyChangesNodeInstance> WeakPointer;

    static Pointer create(QObject *objectToBeWrapped);

    virtual void setPropertyVariant(const QString &name, const QVariant &value);
    virtual void setPropertyBinding(const QString &name, const QString &expression);
    virtual QVariant property(const QString &name) const;
    virtual void resetProperty(const QString &name);

    using ObjectNodeInstance::reparent; // keep the virtual reparent(...) method around
    void reparent(const ServerNodeInstance &oldParentInstance, const QString &oldParentProperty, const ServerNodeInstance &newParentInstance, const QString &newParentProperty);

protected:
    QmlPropertyChangesNodeInstance(QQuickPropertyChanges *object);
    QQuickPropertyChanges *changesObject() const;
};

} // namespace Internal
} // namespace QmlDesigner

//QML_DECLARE_TYPE(QmlDesigner::Internal::QmlPropertyChangesObject)

#endif // QMLPROPERTYCHANGESNODEINSTANCE_H
