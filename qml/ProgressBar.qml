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

Item {
    property double value: 0.0
    property alias interactive: maProgressBar.enabled
    property int _userX;

    signal clicked(double value)

    id: progressBar

    Rectangle {
        anchors.verticalCenter: parent.verticalCenter
        color: "#a99d97"
        width: parent.width
        height: 6
        radius: 2
    }

    Rectangle {
        x: maProgressBar.pressed ? _userX - width / 2 : value * parent.width - width / 2
        anchors.verticalCenter: parent.verticalCenter
        width: 16
        height: 16
        radius: 8
        color: "orange"

        Behavior on x {
            NumberAnimation { duration: 200 }
        }
    }

    MouseArea {
        id: maProgressBar
        anchors.fill: parent
        onPressed: {
            progressBar._userX = Math.min(width, Math.max(0, mouseX));
        }

        onPositionChanged: {
            if (pressed)
            {
                progressBar._userX = Math.min(width, Math.max(0, mouseX));
            }
        }

        onReleased: {
            var value = Math.min(width, Math.max(0, mouseX)) / width;
            progressBar.clicked(value);
        }
    }
}
