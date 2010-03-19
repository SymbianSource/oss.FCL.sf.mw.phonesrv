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
#

TEMPLATE = lib
TARGET = dialpad
CONFIG += hb

INCLUDEPATH += . inc

HEADERS += ../../phonesrv_plat/dialpad_api/inc/dialpad.h \
           inc/dialpadbutton.h \
           inc/dialpadbuttonstyle.h \
           inc/dialpadinputfield.h \
           inc/dialpadkeypad.h \
           inc/dialpadmultitaphandler.h

SOURCES += src/dialpad.cpp \
           src/dialpadbutton.cpp \
           src/dialpadbuttonstyle.cpp \
           src/dialpadinputfield.cpp \
           src/dialpadkeypad.cpp \
           src/dialpadmultitaphandler.cpp

RESOURCES += resources/dialpad.qrc

symbian {    
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
    TARGET.UID3 = 0x102078ED
    
    defFiles = \
        "$${LITERAL_HASH}ifdef WINS" \
            "DEFFILE bwins/dialpad.def" \
        "$${LITERAL_HASH}else" \
            "DEFFILE eabi/dialpad.def" \
        "$${LITERAL_HASH}endif"
    
    MMP_RULES += defFiles
    
    BLD_INF_RULES.prj_exports += \
    "$${LITERAL_HASH}include <platform_paths.hrh>" \
    "./rom/dialpad.iby    CORE_MW_LAYER_IBY_EXPORT_PATH(dialpad.iby)"    
}
else:win32 { 
    # Put it to Hb/lib because it is in path
    DESTDIR = c:/hb/lib
    DLLDESTDIR = c:/hb/bin
    INCLUDEPATH += ../../phonesrv_plat/dialpad_api/inc \
        c:/hb/include/hbcore \
        c:/hb/include/hbwidgets \
        c:/hb/include/hbtools
}

DEFINES += BUILD_DIALPAD
