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



#ifndef QMLDESIGNER_ANCHORCHANGESNODEINSTANCE_H
#define QMLDESIGNER_ANCHORCHANGESNODEINSTANCE_H

#include "objectnodeinstance.h"

#include <QPair>
#include <QWeakPointer>

QT_BEGIN_NAMESPACE
class QQmlProperty;
QT_END_NAMESPACE

namespace QmlDesigner {

namespace Internal {

class AnchorChangesNodeInstance : public ObjectNodeInstance
{
public:
    typedef QSharedPointer<AnchorChangesNodeInstance> Pointer;
    typedef QWeakPointer<AnchorChangesNodeInstance> WeakPointer;

    static Pointer create(QObject *objectToBeWrapped);

    virtual void setPropertyVariant(const QString &name, const QVariant &value);
    virtual void setPropertyBinding(const QString &name, const QString &expression);
    virtual QVariant property(const QString &name) const;
    virtual void resetProperty(const QString &name);

    using ObjectNodeInstance::reparent; // keep the virtual reparent(...) method around
    void reparent(const ServerNodeInstance &oldParentInstance,
                  const QString &oldParentProperty,
                  const ServerNodeInstance &newParentInstance,
                  const QString &newParentProperty);

protected:
    AnchorChangesNodeInstance(QObject *object);
    QObject *changesObject() const;
};

} // namespace Internal
} // namespace QmlDesigner

#endif // QMLDESIGNER_ANCHORCHANGESNODEINSTANCE_H
