#
# Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: ut_vmbxcpplugin.exe
#

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib

symbian: { 
    TARGET.CAPABILITY = CAP_GENERAL_DLL 
}

symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
                   /epoc32/include/mw/QtTest \
                  ../../inc\
                  ./inc
                  TARGET.EPOCALLOWDLLDATA = 1
LIBS += -lcpframework
}

# Input
HEADERS += inc\ut_vmbxuiengine.h\
           inc\ut_vmbxcpplugin.h \
           ..\inc\vmbxuiengine.h \
           ..\inc\vmbxcpplugin.h \
           ..\inc\vmbxcpgroup.h


SOURCES += src\main.cpp\
           src\ut_vmbxuiengine.cpp\
           src\ut_vmbxcpplugin.cpp \
           src\ut_dummyvoicemailbox.cpp\
           src\ut_dummyvoicemailboxentry.cpp\
           ..\src\vmbxuiengine.cpp \
           ..\src\vmbxcpplugin.cpp \
           ..\src\vmbxcpgroup.cpp 
