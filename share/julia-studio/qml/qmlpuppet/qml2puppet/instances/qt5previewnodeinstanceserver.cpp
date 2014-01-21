/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "qt5previewnodeinstanceserver.h"

#include "nodeinstanceclientinterface.h"
#include "statepreviewimagechangedcommand.h"
#include "createscenecommand.h"
#include "removesharedmemorycommand.h"
#include <QQuickView>
#include <QQuickItem>
#include <designersupport.h>

namespace QmlDesigner {

Qt5PreviewNodeInstanceServer::Qt5PreviewNodeInstanceServer(NodeInstanceClientInterface *nodeInstanceClient) :
    Qt5NodeInstanceServer(nodeInstanceClient)
{
}

void Qt5PreviewNodeInstanceServer::createScene(const CreateSceneCommand &command)
{
    initializeView(command.imports());
    setupScene(command);

    startRenderTimer();
}
void Qt5PreviewNodeInstanceServer::startRenderTimer()
{
    if (timerId() != 0)
        killTimer(timerId());

    int timerId = startTimer(renderTimerInterval());

    setTimerId(timerId);
}

void Qt5PreviewNodeInstanceServer::collectItemChangesAndSendChangeCommands()
{
    static bool inFunction = false;

    if (rootNodeInstance().internalSGItem() == 0)
        return;

    if (!inFunction && nodeInstanceClient()->bytesToWrite() < 10000) {
        inFunction = true;

        DesignerSupport::polishItems(quickView());

        QVector<ImageContainer> imageContainerVector;
        imageContainerVector.append(ImageContainer(0, renderPreviewImage(), -1));

        foreach (ServerNodeInstance instance,  rootNodeInstance().stateInstances()) {
            instance.activateState();
            QImage previewImage = renderPreviewImage();
            if (!previewImage.isNull())
                imageContainerVector.append(ImageContainer(instance.instanceId(), renderPreviewImage(), instance.instanceId()));
            instance.deactivateState();
        }

        nodeInstanceClient()->statePreviewImagesChanged(StatePreviewImageChangedCommand(imageContainerVector));

        slowDownRenderTimer();
        inFunction = false;
    }
}

void Qt5PreviewNodeInstanceServer::changeState(const ChangeStateCommand &/*command*/)
{

}

static void updateDirtyNodeRecursive(QQuickItem *parentItem)
{
    foreach (QQuickItem *childItem, parentItem->childItems())
        updateDirtyNodeRecursive(childItem);

    DesignerSupport::updateDirtyNode(parentItem);
}

QImage Qt5PreviewNodeInstanceServer::renderPreviewImage()
{
    updateDirtyNodeRecursive(rootNodeInstance().internalSGItem());

    QRectF boundingRect = rootNodeInstance().boundingRect();

    QSize previewImageSize = boundingRect.size().toSize();
    previewImageSize.scale(QSize(100, 100), Qt::KeepAspectRatio);

    QImage previewImage;

    if (boundingRect.isValid() && rootNodeInstance().internalSGItem())
        previewImage = designerSupport()->renderImageForItem(rootNodeInstance().internalSGItem(), boundingRect, previewImageSize);

    previewImage = previewImage.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    return previewImage;
}

void QmlDesigner::Qt5PreviewNodeInstanceServer::removeSharedMemory(const QmlDesigner::RemoveSharedMemoryCommand &command)
{
    if (command.typeName() == "Image")
        ImageContainer::removeSharedMemorys(command.keyNumbers());
}

} // namespace QmlDesigner
