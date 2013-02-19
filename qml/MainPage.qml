import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    Rectangle {
        id: frame
        anchors.fill: parent
        color: "black"

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
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            onSettingsClicked: {
                albumView.state = "";
                coverFlow.isSelected = false;
                coverFlow.close();
                pageStack.push(settingsPage);
            }
        }

        Page {
            id: settingsPage

            SettingsMenu {
                id: settingsMenu
                anchors.fill: parent

            }

            Rectangle {
                anchors.fill: parent
                visible: !context.isReady
                color: "#dd000000"

                Text {
                    id: busyText
                    anchors.centerIn: parent
                    color: "white"
                    font.pixelSize: 32
                    text: "Working..."
                }

                BusyIndicator {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: busyText.bottom
                    anchors.topMargin: 30
                    running: parent.visible
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        // just absorb the mouse events
                    }
                }
            }

            tools: ToolBarLayout {
                ToolIcon {
                    iconId: 'icon-m-toolbar-back-white'
                    onClicked: pageStack.pop()
                }
            }
        }
    }
}
