/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*/
#include <e32base.h>
#include <QObject>
#include <QtTest/QtTest>
#include "ut_vmbxengine.h"
#include "ut_pbkstore.h"
#include "ut_simhandler.h"

int main(int argc, char *argv[])
{
    qDebug("main() IN");
    CTrapCleanup* cleanup = CTrapCleanup::New();
    QApplication app(argc, argv);
    
    qDebug("testing start...");
    
    // API tester
    Ut_VmbxEngine tc1;
    char *pass[3];  
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxengine.txt";
    int result = -1;
    result = QTest::qExec(&tc1, 3, pass);
    qDebug("ut_vmbxengine result=%d", result);
    
    Ut_PbkStore tc2;
    pass[2] = "c:\\logs\\vmbx\\ut_pbkstore.txt";
    result = QTest::qExec(&tc2, 3, pass);
    qDebug("Ut_PbkStore result=%d", result);
    
    // SIM can not be handled in Emulator.
    // Root cause: CVmbxSimHandler::ConstructL()
//    Ut_SimHandler tc3;
//    pass[2] = "c:\\logs\\vmbx\\ut_simhandler.txt";
//    result = QTest::qExec(&tc3, 3, pass);
//    qDebug("Ut_SimHandler result=%d", result);
    
    qDebug("testing end...");
    qDebug("main() OUT");
    delete cleanup;
    cleanup = NULL;
    return result;
}

