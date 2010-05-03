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
# Description: unit test pro file for satapp.
#

TEMPLATE = app
TARGET = utsatapp
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += hb
CONFIG += qtestlib
HEADERS += $$PUBLIC_HEADERS
MOC_DIR = moc_utsatapp

symbian {
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
                   .inc \
                    ../../../../inc \
                    .MOC_DIR 

    LIBS += -lSatClient.dll \
            -lSatInternalClient.dll \
            -legul.dll \
            -lfbscli.dll \
            -lcentralrepository.dll \
            -lCenRepNotifHandler.dll \
            -lCdlEngine.dll \
            -lFlogger.dll \
            -lbafl.dll \
            -lprofileeng.dll 

    HEADERS += ../../inc \
               ../../inc/msatuiactionimplementer.h \
               ../../inc/satappcommonconstant.h \
               ../../inc/tflogger.h \
               ../../inc/satappmainhandler.h \
               ../../inc/csatuiobserver.h\
               ../../inc/satappcommandhandler.h \
               ../../inc/satappeventprovider.h \
               ../../inc/satappuiprovider.h \
               ../../inc/satappview.h \
               ../../inc/satappgetinkeynote.h \
               ../../inc/csatuiobserver.h \
               ../../inc/csatuiiconhandler.h \
               ../../inc/satappplaytoneprovider.h \
               inc/dummyeventprovider.h \
               inc/dummyplaytoneprovider.h \
               inc/ut_satappmainhandler.h \
               inc/ut_csatuiobserver.h \
               inc/ut_satappeventhandler.h \
               inc/ut_playtoneprovider.h 


    SOURCES +=  ../../src/csatuiobserver.cpp \
               ../../src/satappmainhandler.cpp \
               ../../src/satappcommandhandler.cpp \
               ../../src/satappuiprovider.cpp \
               ../../src/satappeventprovider.cpp \
               ../../src/satappview.cpp \
               ../../src/satappgetinkeynote.cpp \
               ../../src/csatuiiconhandler.cpp\
               ../../src/satappplaytoneprovider.cpp\
               src/main.cpp \
               src/dummyeventprovider.cpp \
               src/dummyplaytoneprovider.cpp \
               src/ut_satappmainhandler.cpp \
               src/ut_csatuiobserver.cpp \
               src/ut_satappeventhandler.cpp \
               src/ut_playtoneprovider.cpp 

}
