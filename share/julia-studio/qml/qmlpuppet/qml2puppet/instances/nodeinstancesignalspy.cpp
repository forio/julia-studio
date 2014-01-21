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

#include "nodeinstancesignalspy.h"
#include "objectnodeinstance.h"

#include <QMetaProperty>
#include <QMetaObject>
#include <QDebug>
#include <QSharedPointer>
#include <private/qqmlmetatype_p.h>

namespace QmlDesigner {
namespace Internal {

NodeInstanceSignalSpy::NodeInstanceSignalSpy() :
    QObject()
{
    blockSignals(true);
}

void NodeInstanceSignalSpy::setObjectNodeInstance(const ObjectNodeInstance::Pointer &nodeInstance)
{
    methodeOffset = QObject::staticMetaObject.methodCount() + 1;
    registerObject(nodeInstance->object());
    m_objectNodeInstance = nodeInstance;

}

void NodeInstanceSignalSpy::registerObject(QObject *spiedObject, const QString &prefix)
{
    if (m_registeredObjectList.contains(spiedObject)) // prevent cycles
        return;

    m_registeredObjectList.append(spiedObject);
    for (int index = QObject::staticMetaObject.propertyOffset();
         index < spiedObject->metaObject()->propertyCount();
         index++) {
             QMetaProperty metaProperty = spiedObject->metaObject()->property(index);

             // handle dot properties and connect the signals to the object
             if (metaProperty.isReadable()
                 && !metaProperty.isWritable()
                 && QQmlMetaType::isQObject(metaProperty.userType())) {
                  QObject *propertyObject = QQmlMetaType::toQObject(metaProperty.read(spiedObject));
                  if (propertyObject)
                      registerObject(propertyObject, prefix + metaProperty.name() + QLatin1Char('.'));
             } else if (metaProperty.hasNotifySignal()) {
                 QMetaMethod metaMethod = metaProperty.notifySignal();
                 bool isConnecting = QMetaObject::connect(spiedObject, metaMethod.methodIndex(), this, methodeOffset, Qt::DirectConnection);
                 Q_ASSERT(isConnecting);
                 Q_UNUSED(isConnecting);
                 m_indexPropertyHash.insert(methodeOffset, prefix + metaProperty.name());
                 methodeOffset++;
             }

             // search recursive in objects
             if (metaProperty.isReadable()
                 && metaProperty.isWritable()
                 && QQmlMetaType::isQObject(metaProperty.userType())) {
                 QObject *propertyObject = QQmlMetaType::toQObject(metaProperty.read(spiedObject));
                 if (propertyObject)
                     registerObject(propertyObject, prefix + metaProperty.name() + QLatin1Char('/'));
             }

             // search recursive in objects list
             if (metaProperty.isReadable()
                 && QQmlMetaType::isList(metaProperty.userType())) {
                 QQmlListReference list(spiedObject, metaProperty.name());

                 if (list.canCount() && list.canAt()) {

                     for (int i = 0; i < list.count(); i++) {
                         QObject *propertyObject = list.at(i);
                         if (propertyObject)
                             registerObject(propertyObject, prefix + metaProperty.name() + QLatin1Char('/'));
                     }
                 }
             }
         }
}

int NodeInstanceSignalSpy::qt_metacall(QMetaObject::Call call, int methodId, void **a)
{
    if (call == QMetaObject::InvokeMetaMethod && methodId > QObject::staticMetaObject.methodCount()) {
        ObjectNodeInstance::Pointer nodeInstance = m_objectNodeInstance.toStrongRef();

        if (nodeInstance && nodeInstance->nodeInstanceServer() && nodeInstance->isValid()) {
            nodeInstance->nodeInstanceServer()->notifyPropertyChange(nodeInstance->instanceId(), m_indexPropertyHash.value(methodId));
        }

    }

    return QObject::qt_metacall(call, methodId, a);
}

} // namespace Internal
} // namespace QmlDesigner
