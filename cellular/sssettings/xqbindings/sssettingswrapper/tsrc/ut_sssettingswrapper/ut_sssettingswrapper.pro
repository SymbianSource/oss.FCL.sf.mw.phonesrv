#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
# 
# Description: Project file for building unit test component
#
#

CONFIG += qtestlib
TEMPLATE = app
TARGET = 
DEPENDPATH += . ../../src/
INCLUDEPATH += .
DEFINES += BUILD_SSSETTINGSWRAPPER

QT -= gui
QT += testlib

symbian: {
  CONFIG += no_icon
  INCLUDEPATH += /epoc32/include/mw/QtTest    \
                 /epoc32/include/platform/mw  \
                 /epoc32/include/mw/qt
  LIBS += -lmocklib -lsymbianmock
}

# test code
HEADERS += ut_sssettingswrapper.h
SOURCES += ut_sssettingswrapper.cpp

# code to be tested
HEADERS += ../../../../../../phonesrv_plat/ss_settings_api/inc/sssettingswrapper.h
SOURCES += ../../src/sssettingswrapper.cpp    \
           ../../src/sssettingswrapper_p.cpp

# mocks needed for testing
SOURCES += ../mocks/mock_rsssettings.cpp
