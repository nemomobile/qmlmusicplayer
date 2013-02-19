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


#include "window.h"

#include <QGLWidget>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QApplication>
#include <QDebug>


Window::Window(bool useGl, QWidget *parent)
    : QDeclarativeView(parent)
{
    if (useGl)
        setViewport(new QGLWidget);

    //resize(200, 200);
    //setResizeMode(QDeclarativeView::SizeRootObjectToView);
    rootContext()->setContextProperty("window", this);
}

bool Window::fullscreen()
{
    return windowState() & Qt::WindowFullScreen;
}

void Window::setFullscreen(bool value)
{
    if (value)
        setWindowState(windowState() | Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    fullscreenChanged(value);
}

bool Window::maximized()
{
    return windowState() & Qt::WindowMaximized;
}

void Window::setMaximized(bool value)
{
    if (value)
        setWindowState(windowState() | Qt::WindowMaximized);
    else
        setWindowState(windowState() & ~Qt::WindowMaximized);
}

bool Window::decorated()
{
    return windowFlags() & Qt::FramelessWindowHint;
}

void Window::setDecorated(bool value)
{
    if (value)
        setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
    else
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    show();
}

void Window::closeWindow()
{
    qDebug() << "closing";
    QApplication::quit();
}
