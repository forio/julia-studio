/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#include "stackhandler.h"

#include "debuggeractions.h"
#include "debuggeragents.h"
#include "debuggerengine.h"

#include <utils/qtcassert.h>
#include <utils/savedaction.h>

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

namespace Debugger {
namespace Internal {

////////////////////////////////////////////////////////////////////////
//
// StackHandler
//
////////////////////////////////////////////////////////////////////////

StackHandler::StackHandler(DebuggerEngine *engine)
  : m_positionIcon(QIcon(QLatin1String(":/debugger/images/location_16.png"))),
    m_emptyIcon(QIcon(QLatin1String(":/debugger/images/debugger_empty_14.png")))
{
    m_engine = engine;
    m_disassemblerViewAgent = new DisassemblerViewAgent(engine);
    m_currentIndex = 0;
    m_canExpand = false;
    connect(theDebuggerAction(OperateByInstruction), SIGNAL(triggered()),
        this, SLOT(resetModel()));
}

StackHandler::~StackHandler()
{
    delete m_disassemblerViewAgent;
}

int StackHandler::rowCount(const QModelIndex &parent) const
{
    // Since the stack is not a tree, row count is 0 for any valid parent
    return parent.isValid() ? 0 : (m_stackFrames.size() + m_canExpand);
}

int StackHandler::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 5;
}

QVariant StackHandler::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case EngineStateRole:
            return m_engine->state();

        case EngineCapabilitiesRole:
            return m_engine->debuggerCapabilities();

        case EngineActionsEnabledRole:
            return m_engine->debuggerActionsEnabled();
    }

    if (!index.isValid() || index.row() >= m_stackFrames.size() + m_canExpand)
        return QVariant();

    if (index.row() == m_stackFrames.size()) {
        if (role == Qt::DisplayRole && index.column() == 0)
            return tr("...");
        if (role == Qt::DisplayRole && index.column() == 1)
            return tr("<More>");
        if (role == Qt::DecorationRole && index.column() == 0)
            return m_emptyIcon;
        return QVariant();
    }

    const StackFrame &frame = m_stackFrames.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0: // Stack frame level
            return QString::number(frame.level);
        case 1: // Function name
            return frame.function;
        case 2: // File name
            return frame.file.isEmpty() ? frame.from : QFileInfo(frame.file).fileName();
        case 3: // Line number
            return frame.line >= 0 ? QVariant(frame.line) : QVariant();
        case 4: // Address
            if (frame.address)
                return QString::fromAscii("0x%1").arg(frame.address, 0, 16);
            return QString();
        }
        return QVariant();
    }

    if (role == Qt::DecorationRole && index.column() == 0) {
        // Return icon that indicates whether this is the active stack frame
        return (index.row() == m_currentIndex) ? m_positionIcon : m_emptyIcon;
    }

    if (role == StackFrameAddressRole)
        return frame.address;

    if (role == Qt::ToolTipRole)
        return frame.toToolTip();

    return QVariant();
}


bool StackHandler::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
        case RequestReloadFullStackRole:
        case RequestActivateFrameRole:
            m_engine->handleCommand(role, value);
            return true;

        case RequestShowMemoryRole:
            (void) new MemoryViewAgent(m_engine, value.toString());
            return true;
    
        case RequestShowDisassemblerRole: {
            const StackFrame &frame = m_stackFrames.at(value.toInt());
            m_disassemblerViewAgent->setFrame(frame);
            return true;
        }
    
        default:
            return QAbstractTableModel::setData(index, value, role);
    }
}

QVariant StackHandler::headerData(int section, Qt::Orientation orient, int role) const
{
    if (orient == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
            case 0: return tr("Level");
            case 1: return tr("Function");
            case 2: return tr("File");
            case 3: return tr("Line");
            case 4: return tr("Address");
        };
    }
    return QVariant();
}

Qt::ItemFlags StackHandler::flags(const QModelIndex &index) const
{
    if (index.row() >= m_stackFrames.size() + m_canExpand)
        return 0;
    if (index.row() == m_stackFrames.size())
        return QAbstractTableModel::flags(index);
    const StackFrame &frame = m_stackFrames.at(index.row());
    const bool isValid = (frame.isUsable() && !frame.function.isEmpty())
        || theDebuggerBoolSetting(OperateByInstruction);
    return isValid ? QAbstractTableModel::flags(index) : Qt::ItemFlags(0);
}

StackFrame StackHandler::currentFrame() const
{
    QTC_ASSERT(m_currentIndex >= 0, return StackFrame());
    QTC_ASSERT(m_currentIndex < m_stackFrames.size(), return StackFrame());
    return m_stackFrames.at(m_currentIndex);
}

void StackHandler::setCurrentIndex(int level)
{
    if (level == m_currentIndex)
        return;

    // Emit changed for previous frame
    QModelIndex i = index(m_currentIndex, 0);
    emit dataChanged(i, i);

    m_currentIndex = level;

    // Emit changed for new frame
    i = index(m_currentIndex, 0);
    emit dataChanged(i, i);
}

void StackHandler::removeAll()
{
    m_stackFrames.clear();
    m_currentIndex = 0;
    reset();
}

void StackHandler::setFrames(const StackFrames &frames, bool canExpand)
{
    m_canExpand = canExpand;
    m_stackFrames = frames;
    if (m_currentIndex >= m_stackFrames.size())
        m_currentIndex = m_stackFrames.size() - 1;
    reset();
}

const StackFrames &StackHandler::frames() const
{
    return m_stackFrames;
}

bool StackHandler::isDebuggingDebuggingHelpers() const
{
    for (int i = m_stackFrames.size(); --i >= 0; )
        if (m_stackFrames.at(i).function.startsWith(QLatin1String("qDumpObjectData")))
            return true;
    return false;
}


} // namespace Internal
} // namespace Debugger
