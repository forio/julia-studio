/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
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
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/


#ifndef QMLDESIGNER_REMOVESHAREDMEMORYCOMMAND_H
#define QMLDESIGNER_REMOVESHAREDMEMORYCOMMAND_H

#include <QMetaType>
#include <QString>
#include <QVector>

namespace QmlDesigner {

class RemoveSharedMemoryCommand
{
    friend QDataStream &operator>>(QDataStream &in, RemoveSharedMemoryCommand &command);

public:
    RemoveSharedMemoryCommand();
    RemoveSharedMemoryCommand(const QString &typeName, const QVector<qint32> &keyNumberVector);

    QString typeName() const;
    QVector<qint32> keyNumbers() const;

private:
    QString m_typeName;
    QVector<qint32> m_keyNumberVector;
};

QDataStream &operator<<(QDataStream &out, const RemoveSharedMemoryCommand &command);
QDataStream &operator>>(QDataStream &in, RemoveSharedMemoryCommand &command);

} // namespace QmlDesigner

Q_DECLARE_METATYPE(QmlDesigner::RemoveSharedMemoryCommand)

#endif // QMLDESIGNER_REMOVESHAREDMEMORYCOMMAND_H
