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


import Qt 4.7

Rectangle {

    Component.onCompleted: {
        //window.maximized = true;
        window.decorated = false;
    }

    property alias toolitems: toolbarContents.children
    property alias title: titleText.text
    property alias contents: container.children
    property alias toolbarVisible: toolbar.visible

    signal closed()

    id: frame

    width: 1252
    height: 768
    anchors.fill: parent

    color: "white"

    Rectangle {
        id: toolbar
        width: 116
        height: parent.height
        anchors.left: parent.left
        color: "#313131"

        Column {
            id: toolbarContents
            anchors.fill: parent
            anchors.topMargin: 8
            spacing: 8

        }
    }

    Rectangle {
        id: titlebar
        height: 43
        color: "#313131"
        anchors.left: toolbar.visible ? toolbar.right : parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        visible: parent.width <= 1252

        Text {
            id: titleText
            color: "white"
            x: 8
            font.pixelSize: 15
            anchors.verticalCenter: parent.verticalCenter

            onTextChanged: {
                window.title = text
            }
        }

        Image {
            id: btnClose
            source: "window-close.png"
            anchors.right: parent.right
            anchors.top: parent.top

            MouseArea {
                anchors.fill: parent
                onClicked: { closed(); }
            }
        }
    }

    Item {
        id: container
        anchors.left: toolbar.visible ? toolbar.right : parent.left
        anchors.top: titlebar.visible ? titlebar.bottom : parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }
}
