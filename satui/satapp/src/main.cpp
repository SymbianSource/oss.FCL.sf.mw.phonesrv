/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*
*/

#include <QApplication>

#include <hbapplication.h>
#include <hbmainwindow.h>
#include <QTranslator>
#include "satappmainhandler.h"

#include "tflogger.h"

int main(int argc, char *argv[])
{
    TFLOGSTRING("SATAPP: main")

    HbApplication app(argc, argv);
    TFLOGSTRING("SATAPP: main app created")
    
    QTranslator translator;
    bool ok = translator.load(":/translations/satapp_en");
    TFLOGSTRING2("SATUI: main, translation %d (1=OK, 0=fail)", ok )
    app.installTranslator(&translator);

    HbMainWindow window;
    TFLOGSTRING("SATAPP: main window created")
    
    // the code is only for test, currently, dphone79 only support
    // lanscape, remove it before official release
    #ifndef __WINSCW
    //window.setOrientation(Qt::Horizontal);
    #endif

    SatAppMainHandler *mainHandler = new SatAppMainHandler(window);
        
    window.show();
    TFLOGSTRING("SATAPP: main view showed")

    int result = app.exec();
    TFLOGSTRING2("SATAPP: main exit %d", result)

    delete mainHandler;
    return result;
}

//End of file
