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
#

TEMPLATE = app
TARGET = satapp
DEPENDPATH += .
HEADERS += $$PUBLIC_HEADERS
MOC_DIR = moc

CONFIG += hb

ICON = resource/qgn_menu_sat.svg
symbian: {
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.UID3=0x101f4ce0
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
        .inc\
        MOC_DIR \
        ../../inc 
    LIBS += -lSatClient.dll \
            -lSatInternalClient.dll \
            -legul.dll \
            -lfbscli.dll \
            -lcentralrepository.dll \
            -lCenRepNotifHandler.dll \
            -lCdlEngine.dll \
            -lFlogger.dll \
            -lprofileeng.dll 

    HEADERS += inc/msatuiactionimplementer.h \
               inc/satappcommonconstant.h \
               inc/tflogger.h \
               inc/satappmainhandler.h \
               inc/satappcommandhandler.h \
               inc/satappeventprovider.h \
               inc/satappuiprovider.h \
               inc/satappview.h \
               inc/satappgetinkeynote.h \
               inc/csatuiobserver.h \
               inc/csatuiiconhandler.h \
               inc/satappplaytoneprovider.h

    SOURCES += src/main.cpp \
               src/satappmainhandler.cpp \
               src/satappcommandhandler.cpp \
               src/satappeventprovider.cpp \
               src/satappuiprovider.cpp \
               src/satappview.cpp \
               src/satappgetinkeynote.cpp \
               src/csatuiobserver.cpp \
               src/csatuiiconhandler.cpp \
               src/satappplaytoneprovider.cpp
    
    BLD_INF_RULES.prj_exports += "rom/satapp.iby CORE_MW_LAYER_IBY_EXPORT_PATH(satapp.iby)" \
                                 "rom/satapp_stub.sis /epoc32/data/z/system/install/satapp_stub.sis" \
                                 "resource/satapp_en.ts /epoc32/include/platform/qt/translations/satapp_en.ts"
                                 
}

RESOURCES += resource/satapp.qrc

