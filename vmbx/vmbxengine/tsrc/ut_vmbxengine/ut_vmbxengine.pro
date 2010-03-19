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
# Description:
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

    LIBS += -lvmbxengine.dll\
             -lflogger.dll
    TARGET.CAPABILITY = ALL -TCB
}

# Input
HEADERS += $$PUBLIC_HEADERS
HEADERS += ut_vmbxengine.h
SOURCES += ut_vmbxengine.cpp\
          ..\..\src\vmbxuihandler.cpp\
          ..\..\src\vmbxqtuihandler.cpp\
          ..\..\src\voicemailboximpl.cpp\
          ..\..\src\vmbxpbkstore.cpp\
          ..\..\src\vmbxetelconnection.cpp\
          ..\..\src\vmbxsimhandler.cpp\
          ..\..\src\vmbxcenrephandler.cpp\
          ..\..\src\vmbxutilities.cpp\
          ..\..\src\vmbxenginefactory.cpp\
          ..\..\src\vmbxenginebase.cpp\
          ..\..\src\vmbxcsvoiceengine.cpp\
          ..\..\src\vmbxcsvideoengine.cpp\
          ..\..\src\vmbxvoipengine.cpp\
          ..\..\src\vmbxemergencycall.cpp\	
          ..\..\src\vmbxuiutilities.cpp\
          ..\..\src\vmbxcenrepobserver.cpp\
          ..\..\src\vmbxsimstoreobserver.cpp\
          ..\..\src\vmbxsatrefreshobserver.cpp\
          ..\..\src\vmbxobserver.cpp\
          ..\..\src\vmbshandler.cpp\
          ..\..\src\vmspshandler.cpp\
          ..\..\src\vmsettingsuiiconfileprovider.cpp
