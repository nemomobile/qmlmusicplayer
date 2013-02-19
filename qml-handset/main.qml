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

Frame {
    id: frame
    color: "#000000"

    onClosed: {
        window.close();
    }

    title: "Music Shelf"

    contents: Item {
        anchors.fill: parent
        clip: true

        CoverFlow {
            id: coverFlow
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: playerControls.top

            onClicked: {
                coverFlow.isSelected = true;
                albumView.state = "active";
            }
        }

        AlbumView {
            id: albumView
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: playerControls.top

            onBack: {
                albumView.state = "";
                //coverFlow.dimmingFactor = 1.0;
                coverFlow.isSelected = false;
                coverFlow.close();
            }
        }

        NowPlaying {
            id: nowPlaying
            y: player.isPlaying ? playerControls.y - height
                                : playerControls.y
            opacity: albumView.state == "active" ? 0 : 1

            anchors.right: parent.right

            Behavior on y {
                NumberAnimation { duration: 500 }
            }

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    coverFlow.findByPath(playQueue.nowPlaying);
                }
            }
        }

        PlayerControls {
            id: playerControls
            width: parent.width
            height: 80
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            opacity: settingsMenu.visible ? 0 : 1

            onSettingsClicked: {
                albumView.state = "";
                coverFlow.isSelected = false;
                coverFlow.close();
                settingsMenu.state = "active";
            }

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }
        }

        MouseArea {
            anchors.fill: parent
            visible: settingsMenu.state == "active"

            onClicked: {
                settingsMenu.state = ""
            }
        }

        SettingsMenu {
            id: settingsMenu
            width: parent.width - 1
            height: parent.height - 64
            y: 0 - height - 1
            visible: y + height > 0

            states:  [
                State {
                    name: "active"
                    PropertyChanges {
                        target: settingsMenu
                        y: 0
                    }
                }
            ]

            Behavior on y {
                NumberAnimation { duration: 500 }
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: ! context.isReady
            color: "#e0a99d97"

            Text {
                anchors.centerIn: parent
                color: "black"
                font.pixelSize: 32
                text: "Please Wait A Moment"
            }

           MouseArea {
               anchors.fill: parent
               onPressed: {
                   // just absorb the mouse events
               }
           }
        }
    }
}
