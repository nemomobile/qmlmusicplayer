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
import "config.js" as Config

PathView {

    function close() {
        var item = childAt(120, 40);
        if (item && albumModel.count > 0) {
            item.x = item.oldX;
            item.y = item.oldY;
        }
    }

    function findByPath(path)
    {
        for (var i = 0; i < albumModel.count; i++)
        {
            var albumPath = albumModel.getPath(i);
            if (path.match("^" + albumPath) == albumPath)
            {
                coverFlow.currentIndex = i;
                break;
            }
        }
    }

    Component.onCompleted: {
        albumModel.path = "albums:///";
        coverFlow.model = albumModel;
    }

    property double dimmingFactor: 1.0
    property bool isSelected: false
    property int selection
    signal clicked()

    id: coverFlow

    interactive: ! isSelected
    focus: true
    pathItemCount: 6
    dragMargin: 100
    preferredHighlightBegin: 0.5
    preferredHighlightEnd: 0.5
    clip: true

    Text {
        anchors.centerIn: parent
        color: "#e0e0e0"
        font.pixelSize: Config.FONT_SIZE_NORMAL
        width: parent.width
        wrapMode: Text.WordWrap
        visible: albumModel.count == 0
        horizontalAlignment: Text.AlignHCenter
        text: "No Music Found" +
              "\n\n" +
              "- fill your music folder with music files or folders -"+
              "\n\n" +
              "- go to settings menu to specify your music folder -" +
              "\n\n" +
              "- click on 'Scan for Files' whenever you add new music -"
    }

    Behavior on dimmingFactor {
        NumberAnimation { duration: 300 }
    }

    delegate: Cover {
        property bool isSelected: (coverFlow.isSelected && index == coverFlow.selection)
        property int oldX
        property int oldY

        id: cover
        width:  coverFlow.height * 2/3
        source: (image != "") ? ("file://" + image) : "nocover.png"
        dimming: coverFlow.isSelected ? 0 : Math.min(1.0, PathView.dimming * coverFlow.dimmingFactor);
        scale: isSelected ? 1.0 : PathView.itemscale
        z: PathView.z
        visible: isSelected || (! coverFlow.isSelected)

        transform: Rotation {
            axis.x: 0
            axis.y: 1
            axis.z: 0
            origin.x: cover.width / 2
            origin.y: cover.height / 4
            angle: cover.isSelected ? 0 : -25 //PathView.angle


            Behavior on angle {
                NumberAnimation { duration: 300 }
            }

        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                console.log("current idx: " + index);

                if (parent.x != 120)
                {
                    parent.oldX = parent.x;
                    parent.oldY = parent.y;
                    parent.x = 120;
                    parent.y = 40;

                    // TODO: it's not great to hardcode the prefix here,
                    //       but for now we just have this one
                    console.log(albumModel.get(index));
                    tracksModel.path = "albums:///" + albumModel.get(index);
                    coverFlow.selection = index;
                    coverFlow.clicked();
                }
            }
        }

        Behavior on x {
            NumberAnimation { duration: 300 }
        }
        Behavior on y {
            NumberAnimation { duration: 300 }
        }

        Behavior on scale {
            enabled: coverFlow.isSelected
            NumberAnimation { duration: 300 }
        }




        states: [
            State {
                name: "selected"
                PropertyChanges {
                    target: cover
                    x: 120
                    y: 48
                }
            }

        ]
    }



    Path {
        id: threeDeePath

        startX: coverFlow.width -100
        startY: 100
        PathAttribute { name: "itemscale"; value: 0.4 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -25 }
        PathAttribute { name: "angle2"; value: -30 }
        PathAttribute { name: "dimming"; value: 1.0 }

        PathQuad { x: -200; y : coverFlow.height; controlX: coverFlow.width; controlY: coverFlow.height * 3/4 }
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -25 }
        PathAttribute { name: "angle2"; value: -30 }
        PathAttribute { name: "dimming"; value: 0.0 }
    }

    Path {
        id: circlePath

        startX: -100; startY: coverFlow.height
        PathAttribute { name: "itemscale"; value: 0.8 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 80 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0 }

        PathCubic {
            x: coverFlow.width + 100; y: coverFlow.height
            control1X: -30; control1Y: coverFlow.height / 2
            control2X: coverFlow.width + 30; control2Y: coverFlow.height / 2
        }
        PathAttribute { name: "itemscale"; value: 0.8 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -80 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0.0 }
    }


    Path {
        id: simplePath

        startX: -200
        startY: coverFlow.height * 3/4
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 10 }
        PathAttribute { name: "angle"; value: -30 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0 }

        PathLine { x: coverFlow.width + 100; y : coverFlow.height * 3/4 }
        PathAttribute { name: "itemscale"; value: 1.0 }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -30 }
        PathAttribute { name: "angle2"; value: 0 }
        PathAttribute { name: "dimming"; value: 0.8 }
    }

    path: threeDeePath
}
