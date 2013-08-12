/* This file is part of Music Shelf
 * Copyright (C) 2011 Martin Grimme  <martin.grimme _AT_ gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


import QtQuick 2.0
import com.nokia.meego 2.0

import "config.js" as Config

Item {
    anchors.fill: parent
    anchors.margins: 12

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: elements.childrenRect.height
        interactive: comboFolders.state != "open"

        Column {
            id: elements
            width: parent.width
            spacing: 32

            Text {
                width: parent.width
                font.pixelSize: Config.FONT_SIZE_NORMAL
                color: "white"
                text: "Select Your Music Folder:"
            }

            ComboBox {
                id: comboFolders
                width: parent.width
                model: folderModel
                selection: context.musicFolder

                onSelected: {
                    context.musicFolder = name;
                }
            }

            Column {
                anchors.left: parent.left
                anchors.right: parent.right

                spacing: 24

                Button {
                    width: parent.width
                    text: "Scan for Files"
                    onClicked: context.scan()
                }

                Button {
                    width: parent.width
                    text: "Clear Index"
                    onClicked: context.clearIndex()
                }
            }
        }
    }
}
