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

#ifndef ABSTRACTNODEINSTANCE_H
#define ABSTRACTNODEINSTANCE_H

#include "nodeinstanceserver.h"
#include "nodeinstancemetaobject.h"
#include "nodeinstancesignalspy.h"

#include <QPainter>
#include <QSharedPointer>
#include <QWeakPointer>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QQmlContext;
class QQmlEngine;
class QQmlProperty;
class QQmlAbstractBinding;
QT_END_NAMESPACE

namespace QmlDesigner {

class NodeInstanceServer;

namespace Internal {

class QmlGraphicsItemNodeInstance;
class GraphicsWidgetNodeInstance;
class GraphicsViewNodeInstance;
class GraphicsSceneNodeInstance;
class ProxyWidgetNodeInstance;
class WidgetNodeInstance;

class ObjectNodeInstance
{
public:
    typedef QSharedPointer<ObjectNodeInstance> Pointer;
    typedef QWeakPointer<ObjectNodeInstance> WeakPointer;
    explicit ObjectNodeInstance(QObject *object);

    virtual ~ObjectNodeInstance();
    void destroy();
    //void setModelNode(const ModelNode &node);

    static Pointer create(QObject *objectToBeWrapped);
    static QObject *createPrimitive(const QString &typeName, int majorNumber, int minorNumber, QQmlContext *context);
    static QObject *createCustomParserObject(const QString &nodeSource, const QStringList &imports, QQmlContext *context);
    static QObject *createComponent(const QString &componentPath, QQmlContext *context);
    static QObject *createComponentWrap(const QString &nodeSource, const QStringList &imports, QQmlContext *context);

    void setInstanceId(qint32 id);
    qint32 instanceId() const;

    NodeInstanceServer *nodeInstanceServer() const;
    void setNodeInstanceServer(NodeInstanceServer *server);
    virtual void initializePropertyWatcher(const Pointer &objectNodeInstance);
    virtual void initialize(const Pointer &objectNodeInstance);
    virtual void paint(QPainter *painter);
    virtual QImage renderImage() const;

    virtual QObject *parent() const;

    Pointer parentInstance() const;

    virtual void reparent(const ObjectNodeInstance::Pointer &oldParentInstance, const QString &oldParentProperty, const ObjectNodeInstance::Pointer &newParentInstance, const QString &newParentProperty);

    virtual void setId(const QString &id);
    virtual QString id() const;

    virtual bool isQmlGraphicsItem() const;
    virtual bool isGraphicsObject() const;
    virtual bool isTransition() const;
    virtual bool isPositioner() const;
    virtual bool isQuickItem() const;

    virtual bool equalGraphicsItem(QGraphicsItem *item) const;

    virtual QRectF boundingRect() const;

    virtual QPointF position() const;
    virtual QSizeF size() const;
    virtual QTransform transform() const;
    virtual QTransform customTransform() const;
    virtual QTransform sceneTransform() const;
    virtual double opacity() const;

    virtual int penWidth() const;

    virtual bool hasAnchor(const QString &name) const;
    virtual QPair<QString, ServerNodeInstance> anchor(const QString &name) const;
    virtual bool isAnchoredBySibling() const;
    virtual bool isAnchoredByChildren() const;

    virtual double rotation() const;
    virtual double scale() const;
    virtual QList<QGraphicsTransform *> transformations() const;
    virtual QPointF transformOriginPoint() const;
    virtual double zValue() const;

    virtual void setPropertyVariant(const QString &name, const QVariant &value);
    virtual void setPropertyBinding(const QString &name, const QString &expression);
    virtual QVariant property(const QString &name) const;
    virtual void resetProperty(const QString &name);
    virtual void refreshProperty(const QString &name);
    virtual QString instanceType(const QString &name) const;
    QStringList propertyNames() const;

    virtual QList<ServerNodeInstance> childItems() const;

    void createDynamicProperty(const QString &name, const QString &typeName);
    void setDeleteHeldInstance(bool deleteInstance);
    bool deleteHeldInstance() const;

    virtual void updateAnchors();
    virtual void paintUpdate();

    virtual void activateState();
    virtual void deactivateState();

    void populateResetHashes();
    bool hasValidResetBinding(const QString &propertyName) const;
    QQmlAbstractBinding *resetBinding(const QString &propertyName) const;
    QVariant resetValue(const QString &propertyName) const;
    void setResetValue(const QString &propertyName, const QVariant &value);

    QObject *object() const;

    virtual bool hasContent() const;
    virtual bool isResizable() const;
    virtual bool isMovable() const;
    bool isInPositioner() const;
    void setInPositioner(bool isInPositioner);
    virtual void refreshPositioner();

    bool hasBindingForProperty(const QString &name, bool *hasChanged = 0) const;

    QQmlContext *context() const;
    QQmlEngine *engine() const;

    virtual bool updateStateVariant(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QVariant &value);
    virtual bool updateStateBinding(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QString &expression);
    virtual bool resetStateProperty(const ObjectNodeInstance::Pointer &target, const QString &propertyName, const QVariant &resetValue);


    bool isValid() const;
    bool isRootNodeInstance() const;

    virtual void doComponentComplete();

    virtual QList<ServerNodeInstance> stateInstances() const;

    virtual void setNodeSource(const QString &source);

    static QVariant fixResourcePaths(const QVariant &value);

protected:
    void doResetProperty(const QString &propertyName);
    void removeFromOldProperty(QObject *object, QObject *oldParent, const QString &oldParentProperty);
    void addToNewProperty(QObject *object, QObject *newParent, const QString &newParentProperty);
    void deleteObjectsInList(const QQmlProperty &metaProperty);
    QVariant convertSpecialCharacter(const QVariant& value) const;

private:
    QHash<QString, QVariant> m_resetValueHash;
    QHash<QString, QWeakPointer<QQmlAbstractBinding> > m_resetBindingHash;
    QHash<QString, ServerNodeInstance> m_modelAbstractPropertyHash;
    mutable QHash<QString, bool> m_hasBindingHash;
    qint32 m_instanceId;
    QString m_id;

    QPointer<NodeInstanceServer> m_nodeInstanceServer;
    QString m_parentProperty;
    bool m_deleteHeldInstance;
    QPointer<QObject> m_object;
    NodeInstanceMetaObject *m_metaObject;
    NodeInstanceSignalSpy m_signalSpy;
    bool m_isInPositioner;
};

} // namespace Internal
} // namespace QmlDesigner

#endif // ABSTRACTNODEINSTANCE_H
