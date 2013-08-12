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
    property alias screen0: scr0.children
    property alias screen1: scr1.children
    property alias screen2: scr2.children
    property alias screen3: scr3.children
    property alias screen4: scr4.children
    property int selection: 0

    id: screenRow

    Row {
        id: row
        x: -selection * screenRow.width

        Behavior on x {
            NumberAnimation { duration: 200 }
        }

        Item {
            id: scr0
            width: screenRow.width
            height: screenRow.height
            clip: true
        }
        Item {
            id: scr1
            width: screenRow.width
            height: screenRow.height
            clip: true
        }
        Item {
            id: scr2
            width: screenRow.width
            height: screenRow.height
            clip: true
        }
        Item {
            id: scr3
            width: screenRow.width
            height: screenRow.height
        }
        Item {
            id: scr4
            width: screenRow.width
            height: screenRow.height
        }
    }
}
