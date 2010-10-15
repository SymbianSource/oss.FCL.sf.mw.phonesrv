#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:
#
#

TEMPLATE = app
TARGET =
MOC_DIR = moc
CONFIG += qtestlib hb
HB += hbinput
DEFINES += BUILD_DIALPAD \
           BUILD_DIALPADKEYHANDLER

QT -= gui

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += . ../../../inc
    INCLUDEPATH +=../../../../../tsrc/common

    
    HEADERS += ../../../inc/dialpadlineedit.h
    SOURCES += ../../../src/dialpadlineedit.cpp
    
    HEADERS += ut_dialpadlineedit.h
    SOURCES += ut_dialpadlineedit.cpp
    HEADERS += testdialpadlineedit.h
    SOURCES += testdialpadlineedit.cpp
    
    #MOCKED DEPENDENCIES
    SOURCES += ../../../../../tsrc/mocks/orbit/mock_hbwidget.cpp
    SOURCES += ../../../../../tsrc/mocks/orbit/mock_hbapplication.cpp
    SOURCES += ../../../../../tsrc/mocks/orbit/mock_hblineedit.cpp
    SOURCES += ../../../../../tsrc/mocks/orbit/mock_hbabstractedit.cpp
    
    LIBS += -lmocklib -lsymbianmock
}
