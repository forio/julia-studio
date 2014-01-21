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

import QtQuick 1.0
import Bauhaus 1.0

QGroupBox {
    id: fontStyleButtons

    property int buttonWidth: 46
    layout: HorizontalLayout {

        QWidget {
            fixedHeight: 32

            FlagedButton {
                checkable: true
                iconSize.width: 24;
                iconSize.height: 24;
                fixedWidth: buttonWidth
                width: fixedWidth
                fixedHeight: 28
                height: fixedHeight
                styleSheetFile: "styledbuttonleft.css";
                checked: backendValues.font_bold.value;
                backendValue: backendValues.font_bold;

                iconFromFile: flagActive ? "images/bold-h-icon.png" : "images/bold-icon.png"

                onClicked: {
                    backendValues.font_bold.value = checked;
                }

                ExtendedFunctionButton {
                    backendValue:   backendValues.font_bold;
                    y: 7
                    x: 2
                }
            }
            FlagedButton {
                x: buttonWidth
                checkable: true
                fixedWidth: buttonWidth
                width: fixedWidth
                fixedHeight: 28
                iconSize.width: 24;
                iconSize.height: 24;
                height: fixedHeight
                iconFromFile: flagActive ? "images/italic-h-icon.png" : "images/italic-icon.png"

                styleSheetFile: "styledbuttonmiddle.css";
                checked: backendValues.font_italic.value;
                backendValue: backendValues.font_italic;

                onClicked: {
                    backendValues.font_italic.value = checked;
                }

                ExtendedFunctionButton {
                    backendValue:   backendValues.font_italic
                    y: 7
                    x: 2
                }
            }
            FlagedButton {
                x: buttonWidth * 2
                checkable: true
                fixedWidth: buttonWidth
                width: fixedWidth
                fixedHeight: 28
                iconSize.width: 24;
                iconSize.height: 24;
                height: fixedHeight
                iconFromFile:  flagActive ? "images/underline-h-icon.png" : "images/underline-icon.png"

                styleSheetFile: "styledbuttonmiddle.css";
                checked: backendValues.font_underline.value;
                backendValue: backendValues.font_underline;

                onClicked: {
                    backendValues.font_underline.value = checked;
                }

                ExtendedFunctionButton {
                    backendValue:   backendValues.font_underline;
                    y: 7
                    x: 2
                }
            }
            FlagedButton {
                x: buttonWidth * 3
                checkable: true
                fixedWidth: buttonWidth
                width: fixedWidth
                fixedHeight: 28
                iconSize.width: 24;
                iconSize.height: 24;
                height: fixedHeight
                iconFromFile: flagActive ? "images/strikeout-h-icon.png" : "images/strikeout-icon.png"

                styleSheetFile: "styledbuttonright.css";
                checked: backendValues.font_strikeout.value;
                backendValue: backendValues.font_strikeout;

                onClicked: {
                    backendValues.font_strikeout.value = checked;
                }

                ExtendedFunctionButton {
                    backendValue:   backendValues.font_strikeout;
                    y: 7
                    x: 2
                }
            }
        }
    }
}
