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
import "config.js" as Config

Item {
    property string source
    property alias dimming: dimmer.opacity

    width: 400
    height: cover.height * 2

    Rectangle {
        id: title
        anchors.left: parent.left
        anchors.top: parent.top
        width: Math.min(cover.width, label.width + 8)
        height: label.font.pixelSize + 4
        radius: 4
        border.color: "black"
        color: "#a99d97"
        smooth: true
        clip: true

        Text {
            id: label
            x: 4
            font.family: "Arial"
            font.pixelSize: Config.FONT_SIZE_SMALL
            color: "#121212"
            text: name
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            smooth: true
        }
    }

    Image {
        id: cover
        anchors.top: title.bottom
        x: 1
        width: parent.width - 1
        height: parent.width
        sourceSize.width: 400
        sourceSize.height: 400
        source: parent.source
        smooth: true
    }

    Image {
        id: reflection
        x: 1
        width: parent.width - 1
        height: parent.width
        sourceSize.width: 400
        sourceSize.height: 400
        source: parent.source
        anchors.top: cover.bottom
        opacity: 0.5

        transform: Rotation {
            axis.x: 1
            axis.y: 0
            axis.z: 0
            origin.x: reflection.width / 2
            origin.y: reflection.height / 2
            angle: 180
        }
    }

    Rectangle {
        anchors.fill: reflection
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#30000000" }
            GradientStop { position: 0.5; color: "#ff000000" }
            GradientStop { position: 1.0; color: "#ff000000" }
        }
    }

    Rectangle {
        id: dimmer
        color: "black"
        anchors.fill: parent
    }

}
