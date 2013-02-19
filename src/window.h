/* This file is part of MeeGoFrames Component Library
 * Copyright (C) 2011 Martin Grimme  <martin.grimme _AT_ gmail.com>
 *
 * This program is free software; you can redistribute it, even commercially, 
 * as long as this copyright notice remains in place.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */


#ifndef WINDOW_H
#define WINDOW_H

#include <QDeclarativeView>

class QString;

class Window : public QDeclarativeView
{
    Q_OBJECT

    Q_PROPERTY(QString title
               READ windowTitle
               WRITE setWindowTitle);

    Q_PROPERTY(bool fullscreen
               READ fullscreen
               WRITE setFullscreen
               NOTIFY fullscreenChanged);
    Q_PROPERTY(bool maximized
               READ maximized
               WRITE setMaximized);
    Q_PROPERTY(bool decorated
               READ decorated
               WRITE setDecorated);


public:
    explicit Window(bool useGl, QWidget *parent = 0);

private:
    bool fullscreen();
    void setFullscreen(bool value);

    bool maximized();
    void setMaximized(bool value);

    bool decorated();
    void setDecorated(bool value);

signals:
    void fullscreenChanged(bool value);

public slots:
    void closeWindow();

};

#endif // WINDOW_H
