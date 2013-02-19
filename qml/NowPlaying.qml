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

    function extractAlbum(path)
    {
        if (playQueue.nowPlaying == "")
            return "";
        var parts = playQueue.nowPlaying.split("/");
        return parts[1];
    }

    function extractTitle(path)
    {
        if (playQueue.nowPlaying == "")
            return "";
        var parts = playQueue.nowPlaying.split("/");
        return parts[2];
    }

    id: nowPlaying
    color: "#121212a0"
    width: label.paintedWidth + 16
    height: label.paintedHeight + 16

    Text {
        id: label
        x: 8
        y: 8
        color: "white"
        font.pixelSize: 24
        text: extractTitle(playQueue.nowPlaying) + "\n" +
              extractAlbum(playQueue.nowPlaying)
    }

}
