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
    property alias model: list.model
    property string selection
    property int fontSize: 32

    signal selected(string name)

    function _indexOf(name)
    {
        for (var i = 0; i < model.count; i++)
        {
            if (model.get(i) == name)
            {
                return i;
                break;
            }
        }
        return 0;
    }


    id: comboBox
    height: state == "open" ? box.childrenRect.height : 42

    color: "#00000000"
    border.color: "#a99d97"
    border.width: 3
    radius: 5
    clip: true

    Behavior on height {
        NumberAnimation {
            duration: 200
        }
    }

    Column {
        id: box
        x: 8
        width: parent.width - 16

        Item {
            id: selectionBox
            width: parent.width
            height: 42

            Text {
                id: selectionLabel
                width: parent.width
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: comboBox.fontSize
                color: "white"
                text: list.currentItem.text
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    comboBox.state = "open"
                }
            }

        }

        Rectangle {
            width: parent.width
            height: 2
            color: "#a99d97"
        }

        ListView {
            id: list
            width: parent.width
            height: 120
            clip: true
            currentIndex: comboBox._indexOf(comboBox.selection)

            delegate: Text {
                font.pixelSize: comboBox.fontSize
                color: (list.currentIndex == index) ? "orange" : "white"
                text: name

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        list.currentIndex = index;
                        comboBox.state = "";
                        comboBox.selected(name);
                    }
                }
            }
        }
    }

    states: [
        State {
            name: "open"
        }
    ]
}
