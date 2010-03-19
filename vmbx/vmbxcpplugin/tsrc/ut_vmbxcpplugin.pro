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
                  ../../inc

    LIBS += -lvmbxcpplugin.dll\
            -lvmbxengine.dll\
             -lflogger.dll\
             -lcpframework


}

# Input
HEADERS += ut_vmbxcpplugin.h\
          ..\inc\vmbxcpgroup.h\
          ..\inc\vmbxuiengine.h

SOURCES += ut_vmbxcpplugin.cpp\
          ..\src\vmbxcpgroup.cpp\
          ..\src\vmbxuiengine.cpp

