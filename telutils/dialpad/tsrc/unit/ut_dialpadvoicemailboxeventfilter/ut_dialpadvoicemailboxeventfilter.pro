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
INCLUDEPATH += . ../../../inc  ../shared

CONFIG += hb qtestlib

symbian {
    INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE
} else:win32 {
    DESTDIR = ./
    INCLUDEPATH += ../../../../../phonesrv_plat/dialpad_api/inc
    INCLUDEPATH += ../../../../xqtelephonyservice/inc
}

#
HEADERS += ../../../inc/dialpadvoicemailboxeventfilter.h
HEADERS += ../../../inc/dialpadsymbianwrapper.h
HEADERS += xqservicerequest.h
SOURCES += ut_dialpadvoicemailboxeventfilter.cpp
SOURCES += ../../../src/dialpadvoicemailboxeventfilter.cpp

# 
LIBS += -ldialpad