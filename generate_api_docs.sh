#!/usr/bin/env bash

### Setting environment variables ###
export BUILDDIR=`pwd` 
export QT_VERSION=5.15.13 
export QT_VER=5.15.13 
export QT_VERSION_TAG=51513 
export QT_INSTALL_DOCS="/usr/share/doc/qt5/"

### Generate autodocs for libqtldap library ###
qdoc config.qdocconf -I /usr/include/qt5 -I /usr/include/qt5/QtCore/
