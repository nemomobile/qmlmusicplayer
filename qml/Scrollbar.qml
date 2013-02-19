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
    property variant scrollTarget

    anchors.left: parent.left
    anchors.leftMargin: 2
    width: 5
    radius: 2

    color: "#a99d97"
    opacity: height < parent.height ? 1 : 0
    clip: true

    y: scrollTarget.visibleArea.yPosition * parent.height
    height: scrollTarget.visibleArea.heightRatio * parent.height

    Behavior on opacity {
        NumberAnimation { duration: 200 }
    }
}
